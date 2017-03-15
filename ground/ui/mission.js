var moveTo = null;
function createAutomata(jsonSchema){
  var canvas = $("#canvas")[0];
  var context = canvas.getContext('2d');
  var prv_state = undefined;
  var state_counter = 1;
  return function(current_state){
    context.clearRect(0, 0, canvas.width, canvas.height);
    var states = jsonSchema.states;
    var nodesList = {};
    for (var i = 0; i < states.length; i++) {
      var state = states[i];

      var node = new Node(state.x,state.y);
      node.text = state.label
      node.isAcceptState = state.final || false
      node.draw(context)
      if(state.self_link)
      {
        var selfLink = new SelfLink(node);
        selfLink.text = state.self_link
        selfLink.draw(context)
      }
      if(state.start)
      {
        var startArrow = new StartLink(node)
        startArrow.draw(context)
      }
      nodesList[state.id] = node

    }
    for (var i = 0; i < states.length; i++) {
      var transitions = states[i].transitions;
      if(transitions)
      {
        var state = states[i]
        for (var j = 0; j < transitions.length; j++) {
          var transition = transitions[j];
          var link = new Link(nodesList[state.id],nodesList[transition.to])
          link.text = transition.label;
          link.draw(context);
        }
      }
    }
    if(current_state == undefined)
    {
        return;
    }
    context.save()
    context.globalAlpha=0.3;
    var radius = 55;
    centerX = nodesList[current_state].x
    centerY = nodesList[current_state].y
    context.beginPath();
    context.arc(centerX, centerY, radius, 0, 2 * Math.PI, false);
    context.fillStyle = 'yellow';
    context.fill();
    context.restore()
    context.beginPath();
    context.fillStyle = 'red';
    context.arc(centerX + radius - 18, centerY - radius + 18, 12, 0, 2 * Math.PI, false);
    context.fill();
    context.lineWidth = 0.5;
    context.strokeStyle = '#000000';
    context.stroke();
    if(prv_state == current_state)
    {
      state_counter++;
    }
    else
    {
      state_counter = 1;
    }
    var originalText = state_counter;
    var x = centerX + radius - 18;
    var y = centerY - radius + 18;
    context.font = '16px "Times New Roman", serif';
    context.fillStyle = 'black';
    var width = context.measureText(originalText).width;
    if('advancedFillText' in context) {
      context.advancedFillText(originalText, originalText, x - width / 2, y);
    } else {
      x = Math.round(x);
      y = Math.round(y);
      context.fillText(originalText, x - width / 2, y + 6);
    }
    prv_state = current_state;
  }
}
function loadMission(mission){
    $.getJSON("./Missions/" + mission + ".json", function(jsonSchema) {
      $('#mission_title').text(mission)
      moveTo = createAutomata(jsonSchema)
      moveTo()
      $('.cancel').show();
    });
}
var client = new WsClient("127.0.0.1",8080)
