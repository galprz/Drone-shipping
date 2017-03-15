
#include "find_and_land_mission.hpp"

using namespace VehicleModule::Mission;
using namespace VehicleModule::Video;
using namespace VehicleModule::Algorithm;
using namespace std::chrono;
using namespace boost;

#define NUM_OF_RETRIES 10
#define WAIT_TO_VIDEOPROVIDER 500
#define CONTROL_LOOP_TIMEOUT 10000
#define HEIGHT_THRESHOLD 1
#define MAX_HISTORY_SIZE 15
#define NUMBER_OF_RETRIES 3
#define DISTANCE_FROM_CENTER_THRESHOLD 50
#define K 1
#define TAU_1 20
#define TAU_2 20
#define Alpha (45 / (double) 360) * 2 * M_PI

typedef arma::vec TargetError;
typedef arma::vec Location;
typedef deque<TargetError> ErrorsHistory;

FindAndLandMission::FindAndLandMission(double altitude, double distance, double number_of_moves)
: _altitude(altitude), _distance(distance), _number_of_moves(number_of_moves) {
    _add_state(State(0, "Takeoff", static_cast<StateMachine::st_func>(&FindAndLandMission::_takeoff)));
    _add_state(State(1, "Coarse Scan", static_cast<StateMachine::st_func>(&FindAndLandMission::_scan)));
    _add_state(State(2, "Fine Scan", static_cast<StateMachine::st_func>(&FindAndLandMission::_fine_scan)));
    _add_state(State(3, "Land", static_cast<StateMachine::st_func>(&FindAndLandMission::_land), true));
    _add_transition(Transition(0, 1, static_cast<StateMachine::tr_func>(&FindAndLandMission::_should_transit)));
    _add_transition(Transition(1, 2, static_cast<StateMachine::tr_func>(&FindAndLandMission::_should_transit)));
    _add_transition(Transition(2, 3, static_cast<StateMachine::tr_func>(&FindAndLandMission::_should_transit)));
}

bool FindAndLandMission::_takeoff()
{
    bool ret;
    BEGIN_PYTHON_EXECUTION
    PyObject* vehicle_control = (PyObject* ) Common::ServiceProvider::get_instance().get_service("vehicle_control");
    ret =  python::call_method<bool>(vehicle_control, "arm")
    && python::call_method<bool>(vehicle_control, "takeoff", _altitude);
    END_PYTHON_EXECUTION
    return ret;
}

bool FindAndLandMission::_scan()
{
    VideoProvider& video_provider = VideoProvider::get_instance();
    Modifiers::Collection collection;
    std::shared_ptr<Modifiers::Mask::TargetCenter> target_center_mask(new Modifiers::Mask::TargetCenter());
    collection.add_modifier(std::shared_ptr<Modifiers::AbstractModifier>(target_center_mask));
    video_provider.set_channel(VideoProvider::Channel::DEBUG, collection);

    // TODO: calculate correct distance
    int distance = 100; // cm
    int moves_left_in_direction = 1;
    int num_moves_in_direction = 1;
    int j = 0, x = 0, y = distance, sign = 1;
    while (j < 20) {
        int target_counter = 0;
        Common::Logger::debug("Start scanning ..." ,FIND_AND_LAND_TAG);

        for (int i=0; i < NUM_IMAGE_TO_SCAN; i++) {
            cv::Mat frame;
            cv::Point target_center;
            long long image_ts;
            if (video_provider.get_frame(frame, VideoProvider::Channel::DEFAULT, image_ts)) {
                if (find_bullseye_direction(frame, target_center)) {
                    target_counter++;
                    target_center_mask->set_target_center(target_center);
                    Common::Logger::debug("Found target center x:" + std::to_string(target_center.x) + " y:" + std::to_string(target_center.y) ,FIND_AND_LAND_TAG);
                }
            }
            if (target_counter >= TARGET_THRESHOLD) {
                VideoProvider::get_instance().clear_channel(VideoProvider::Channel::DEBUG);
                return true;
            }
            if (!video_provider.new_frame_exist(image_ts)) {
                video_provider.wait_to_next_frame();
            }
        }
        Common::Logger::debug("Did not found enough frames with target . required : " + std::to_string(TARGET_THRESHOLD) + " found : " + std::to_string(target_counter),FIND_AND_LAND_TAG);

        if (moves_left_in_direction-- == 0) {
            num_moves_in_direction += 1 - (j % 2);
            moves_left_in_direction = num_moves_in_direction;
            sign = j++ % 2 == 0 ? sign : -sign;
            int tmp = x;
            x = abs(y) * sign;
            y = abs(tmp) * sign;
        }
        Common::Logger::debug("Moving drone to x:" + std::to_string(x) + ", y:" + std::to_string(y) + " moves left to that direction is " + std::to_string(moves_left_in_direction) ,FIND_AND_LAND_TAG);
        BEGIN_PYTHON_EXECUTION
        PyObject* vehicle_control = (PyObject* ) Common::ServiceProvider::get_instance().get_service("vehicle_control");
        python::call_method<void>(vehicle_control, "goto_xy", x, y);
        END_PYTHON_EXECUTION
    }

    VideoProvider::get_instance().clear_channel(VideoProvider::Channel::DEBUG);
    return false;
}

static TargetError integral(const ErrorsHistory & errors_history){
    Location out(3);
    out[0] = out[1] = out[2] = 0;
    double factor = 2;
    int size = errors_history.size();
    if(size > 2){
        for(auto err : errors_history ){
            out += err / factor;
            factor *= factor;
        }
    }
    return out;
}

static TargetError derivative(const ErrorsHistory & errors_history){
    Location out(3);
    out[0] = out[1] = out[2] = 0;
    double factor = 2;
    int size = errors_history.size() ;
    if(size > 2){
        for(int i = 0 ; i < (size - 1) ; ++i){
            Location err = errors_history[i];
            Location prv_err = errors_history[i+1];
            out += (prv_err - err) / factor;
            factor *= factor;
        }
    }
    return out;
}

static double pixel2meter(int pixels, double height, int frame_width){
    return pixels * 2 * height * tan(0.5 * Alpha) / (double) frame_width;
}

static int meter2pixel(double meter, double height, int frame_width){
    return meter * frame_width / (2 * height * tan(0.5 * Alpha));
}

bool FindAndLandMission::_try_get_accurate_altitude(double & out){
    int number_of_retries = 0;
    PyObject* vehicle_control = (PyObject* ) Common::ServiceProvider::get_instance().get_service("vehicle_control");
    BEGIN_PYTHON_EXECUTION
    while(number_of_retries < NUM_OF_RETRIES){
        double roll_angle = python::call_method<double>(vehicle_control, "get_altitude");
        double pitch_angle = python::call_method<double>(vehicle_control, "get_altitude");
        if(abs(roll_angle) > 0.25 || abs(pitch_angle) > 0.25){
            number_of_retries++;
        }else{
            break;
        }
    }
    out = python::call_method<double>(vehicle_control, "get_altitude");
    END_PYTHON_EXECUTION
    return (number_of_retries == NUM_OF_RETRIES);
}

bool FindAndLandMission::_fine_scan(){
    VideoProvider& video_provider = VideoProvider::get_instance();
    Modifiers::Collection collection;
    std::shared_ptr<Modifiers::Mask::TargetCenter> target_center_mask(new Modifiers::Mask::TargetCenter());
    std::shared_ptr<Modifiers::Mask::DirectionVector> direction_vector_mask(new Modifiers::Mask::DirectionVector());
    collection.add_modifier(std::shared_ptr<Modifiers::AbstractModifier>(target_center_mask))
              .add_modifier(std::shared_ptr<Modifiers::AbstractModifier>(direction_vector_mask));
    video_provider.set_channel(VideoProvider::Channel::DEBUG, collection);
    int retries = 0;
    while((!video_provider.get_width() || !video_provider.get_height()) && retries++ < NUMBER_OF_RETRIES){
        std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TO_VIDEOPROVIDER));
    }
    int frame_width  = video_provider.get_width();
    int frame_height = video_provider.get_height();
    if(!frame_width || !frame_height){
        VideoProvider::get_instance().clear_channel(VideoProvider::Channel::DEBUG);
        Common::Logger::debug("Can get frame width or frame height",FIND_AND_LAND_TAG);
        return false;
    }
    int number_of_retries = 0;
    PyObject* vehicle_control = (PyObject* ) Common::ServiceProvider::get_instance().get_service("vehicle_control");
    arma::vec required_position(3);
    while(!_try_get_accurate_altitude(required_position[2]) && number_of_retries < NUM_OF_RETRIES){
        number_of_retries++;
    }
    if(number_of_retries == NUM_OF_RETRIES){
        VideoProvider::get_instance().clear_channel(VideoProvider::Channel::DEBUG);
        Common::Logger::debug("Can not get accurate altitude",FIND_AND_LAND_TAG);
        return false;
    }
    number_of_retries = 0;
    required_position[0] = frame_width / 2;
    required_position[1] = frame_height / 2;
    ErrorsHistory errors;
    double current_height = 0;
    auto controller_start_time = std::chrono::high_resolution_clock::now();
    cv::Point from(frame_width / 2, frame_height / 2);
    while(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - controller_start_time).count() < CONTROL_LOOP_TIMEOUT){
        while(!_try_get_accurate_altitude(current_height) && number_of_retries < NUM_OF_RETRIES){
            number_of_retries++;
        }
        if(number_of_retries == NUM_OF_RETRIES){
            VideoProvider::get_instance().clear_channel(VideoProvider::Channel::DEBUG);
            Common::Logger::debug("Can not get accurate altitude when trying to find if the vehicle at required height",FIND_AND_LAND_TAG);
            return false;
        }
        if(current_height < HEIGHT_THRESHOLD){
            VideoProvider::get_instance().clear_channel(VideoProvider::Channel::DEBUG);
            Common::Logger::debug("Reached required height , current height is " + std::to_string(current_height),FIND_AND_LAND_TAG);
            return true;
        }
        number_of_retries = 0;
        Common::Logger::debug("Trying to reach desiried height , current height is " + std::to_string(current_height),FIND_AND_LAND_TAG);

        long long image_timestamp;
        cv::Mat frame;
        cv::Point target_center;
        while(!video_provider.get_frame(frame, VideoProvider::Channel::DEFAULT, image_timestamp) || !find_bullseye_direction(frame,target_center)){
            if(number_of_retries++ == NUM_OF_RETRIES){
                VideoProvider::get_instance().clear_channel(VideoProvider::Channel::DEBUG);
                Common::Logger::debug("Can not find target in the picture",FIND_AND_LAND_TAG);
                return false;
            }
            video_provider.wait_to_next_frame();
        }
        Common::Logger::debug("Found target center x:" + std::to_string(target_center.x) + " y:" + std::to_string(target_center.y) ,FIND_AND_LAND_TAG);
        target_center_mask->set_target_center(target_center);
        number_of_retries = 0;
        arma::vec error(3);
        error[0] = pixel2meter((target_center.x  -  required_position[0]), current_height, frame_width);
        error[1] = pixel2meter((target_center.y  -  required_position[1]), current_height, frame_width);
        error[1] = -error[1];
        auto x_value = error[0];
        error[0] = error[1];
        error[1] = -x_value;
        int a = target_center.x  -  required_position[0];
        int b = target_center.y  -  required_position[1];
        double distance_from_center = sqrt(a*a + b*b);
        if(distance_from_center < DISTANCE_FROM_CENTER_THRESHOLD){
            required_position[2] -= 0.5;
            errors.clear();
            Common::Logger::debug("Target center is close enough , lowering height from " + std::to_string(current_height) + "to " + std::to_string(required_position[2]),FIND_AND_LAND_TAG);
        }else{
            Common::Logger::debug("distance from center in px is " + std::to_string(distance_from_center) + "required  distance is " + std::to_string(DISTANCE_FROM_CENTER_THRESHOLD),FIND_AND_LAND_TAG);
        }
        while(!_try_get_accurate_altitude(current_height) && number_of_retries < NUM_OF_RETRIES){
            number_of_retries++;
        }
        if(number_of_retries == NUM_OF_RETRIES){
            VideoProvider::get_instance().clear_channel(VideoProvider::Channel::DEBUG);
            Common::Logger::debug("Can not get accurate altitude when trying to estimate the error",FIND_AND_LAND_TAG);
            return false;
        }
        number_of_retries = 0;
        error[2] = required_position[2] - current_height;
        if(errors.size() > MAX_HISTORY_SIZE){
            errors.pop_back();
        }
        errors.push_front(error);
        arma::vec output =  K * error + (K/TAU_1) * integral(errors) + (K/TAU_2) * derivative(errors);
        cv::Point to(meter2pixel(output[0], current_height, frame_width) + frame_width/2,
                    -meter2pixel(output[1], current_height, frame_width) + frame_height/2);
        direction_vector_mask->set_direction_vector(from,to);
        output *= 100;
        BEGIN_PYTHON_EXECUTION
        python::call_method<void>(vehicle_control, "goto_xyz", output[0], output[1],output[2]);
        END_PYTHON_EXECUTION
        if (!video_provider.new_frame_exist(image_timestamp)) {
            video_provider.wait_to_next_frame();
        }
    }
    VideoProvider::get_instance().clear_channel(VideoProvider::Channel::DEBUG);
    return false;
}

bool FindAndLandMission::_land() {
    bool ret;
    BEGIN_PYTHON_EXECUTION
    PyObject* vehicle_control = (PyObject* ) Common::ServiceProvider::get_instance().get_service("vehicle_control");
    ret = python::call_method<bool>(vehicle_control, "land");
    END_PYTHON_EXECUTION
    return ret;
}

bool FindAndLandMission::_should_transit() const {
    return _last_state_succeed;
}
