/// \file     Img.h    \author  Haim Kreitman  \date   2001 B.C.
/// \brief    Img - This class (uses Imgfwd.h) handles images which are two dimensions pixel matrix.
/// \details  Img - This class (uses Imgfwd.h) handles images which are two dimensions pixel matrix.
///           Buffer construction:
///
///           There are two ways to construct the image data :
///
///                1, User supplied pixel-buffer.
///                2. Internal buffer.
///
///           Example:
///               Img<char>   img1(rows,clms);  // use internal buffer
///
///               char bigBuf[10000];
///
///               Img<char>   img2(rows,clms,bigBuf);  // user-supplied buffer
///
///          It supports brackets operations and filling the Img with one particular pixel.
///          Copy operations (Ctor and assignment) are deep copys.
///
///          Three global functions defined : getHeight and getWidth and getBits
///
///          Predefined typedefs:
/// ----------------------------------------------------------------------
///          Img<  signed  char>        ImgChar;  // 1 byte
///          Img<unsigned  char>        ImgByte;  // 1 byte
///          Img<         short>        ImgShrt;  // 2 bytes short
///          Img<unsigned short>        ImgWord;  // 2 bytes unsigned short
///          Img<bool>                  ImgBool;  // 1 bit
///          Img<int>                   ImgLong;  // 4 bytes int
///          Img<unsigned int>          ImgDwrd;  // 4 bytes unsigned int (Double WoRD)
///          Img<float>                 ImgReal;  // 4 bytes float
///          Img<double>                ImgDblp;  // 8 bytes DouBLe Precision
///          Img<ARGBpixel>             ImgARGB;  //32  bit (4 bytes)
///          Img< RGBpixel>             Img_RGB;  //24  bit (3 bytes)
///          Img<HRGBpixel>             ImgHRGB;  //48  bit (3 shorts)
///          Img<WRGBpixel>             ImgWRGB;  //48  bit (3 words)
///          Img<IRGBpixel>             ImgIRGB;  //96  bit (3 ints)
///          Img<FRGBpixel>             ImgFRGB;  //96  bit (3 floats)
///          Img<DRGBpixel>             ImgDRGB;  //192 bit (3 doubles)
///
///              Typedefs : value_type (for pixel)
///                         Pixel      (the same)
///                         ConstPixelPtr
///                         PixelPtr
///                         ConstPixelRef
///                         PixelRef
///                         FlatImg
///
///              Example:   ImgByte::PixelPtr   is   unsigned char*
///                         ImgBool::PixelPtr   is   BitPtr
///
///    !!! See Imgfwd.h  for BitPtr and BitRef to serve ImgBool
/// ----------------------------------------------------------------------------
///       The Img<X> object can contain also additional storage of some number of ints
///                  The size of this storage is:
///                  either 0 or Img<X>::minStorageLen() [static member-function returns 4]
///                  or more (use static member-function Img<X>::xalloc()
///                            exactly as for iostreams, to reserve indexes)
///                  access to this storage is:
///                            int  iword() const            : get # of ints in this img
///                            int& iword(int index)         : allocate if needed
///                      const int& iword(int index) const   : allocate if needed
///                 This additional storage allocated on 1st request and set to 0
///
///       The main purpose is to keep xPelsKM and yPelsKM in indexes 0 and 1
/// ----------------------------------------------------------------------------
///         Class ImgBaseStorage
///       *------------------------*
///       | int*  intArray         |---> 0 or points to array allocated in heap
///       *=================================*
///       |       int  iword()        const |    returns 0 or # of allocated ints
///       | const int& iword(int idx) const |
///       |       int& iword(int idx)       |
///       *=======================================*
///       | static int xalloc()      {return n++;}|
///       | static int minStorageLen() {return 4;}|
///       *---------------------------------------*
///                        ^
///                        |
///    Class ImgBase<X>    |
///       *----------------------------------*
///       | size_t  rows                     |
///       | size_t  clms                     |
///       | PixBox* data (mostly PixBox is X)|----> points to array allocated in heap
///       | bool    myBuf                    |      or supplied as hint (when myBuf==false)
///       *================================================*
///       | const X* operator[](int r) const               |
///       |       X* operator[](int r)                     |
///       | void  complement()        Do rownot on all rows|
///       | void  resize(rows,clms)   All info will be lost|
///       | int   rowsize() const                          |
///       | void  rowcpy(rowNo,from,flipX=false)           |
///       | void  rowset(int r, X pixel)                   |
///       | void  rownot(int r)                            |
///       | void  cpyFrom(imgFrom,flipY=false,flipX=false) |
///       | void fillFrom(imgFrom,flipY=false,flipX=false) |
///       | bool  operator==(anyImg) const                 |
///       | bool  operator==(pixel ) const                 |
///       | bool  isEqual(rowNo,pixel) const               |
///       | bool  isEqual      (pixel) const               |
///       | int   pixBoxesInRow() const                    |
///       *================================================*
///       | static bool rowsRequal(rowAptr,rowBptr,n)|
///       | static int  privPixBoxesInRow(cols)      |
///       *------------------------------------------*
///
/// ----------------------------------------------------------------------------
///       Bitwise operations:
///       ImgBool supports ^= &= |=
///       Any Img<Pixel>  with Pixel, which supports '~' (bitwise complement)
///       also supports '~'
///                 Example:
///                            ImgBool&  imgA = ...
///                      const ImgBool&  imgB = ...
///                          imgA &= imgB;    // AND of 2 images
///                          imgA |= imgB;    //  OR of 2 images
///                          imgA ^= imgB;    // XOR of 2 images
///                          imgA = ~imgB;    // NOT
///
///                      const ImgByte& oldImg = ...
///                            ImgByte  newImg = ~oldImg;
/// ----------------------------------------------------------------------------
///
///       ImgBool also contains static public functions
///
///       int  reverseByte  ( int byte  )
///       void reverseBytes ( void* ou , const void* in, size_t nbits ) !!! nbits: NOT nbytes
///
///
///       member-functions
///       ================
///       void operator=(Pix pix)             // clear all img by pix
///       void resize(int rows, int clms)     // bypass iff rows,clms NOT changed
///       int rowsize() const                 // get number of bytes in row
///
///       template <class AnyPixelPtr>        // copy pixels from any row
///       void rowcpy(int rowNo, AnyPixelPtr from);
///
///       void rowset(int rowNo, Pix pix)     // set row to pix
///       void rownot(int rowNo)              // make ~ on all pixels of row
///
///       template <class DoublyBracketable>  // copy img from ANY DoublyBracketable
///       void cpyFrom(const DoublyBracketable& imgFrom , bool flipY = false)
///                           // which has ::getHeight(imgFrom) and ::getWidth(imgFrom)
///                           // If flipY is true ==> rows will be copied in reverse order
///
///       bool isEqual(Pix pix) const         // true for uniform picture
///       bool operator==(Pix pix) const      { return  isEqual(pix);}
///       bool operator!=(Pix pix) const      { return !isEqual(pix);}
///
///       int pixBoxesInRow() const           // # of 'int' in row of ImgBool
///                                           // in ImgByte,ImgWord ... # of clms in row
///       template<class Pix2>
///       operator Img<Pix2>& ()              // cast to other img type with the
///                                           // SAME pixel size
///       template<class Pix2>
///       operator const Img<Pix2>& () const  // cast to other img type with the
///                                           // SAME pixel size
///
///     Usage:
/// ================
/// \code
///      Img <int> i = Img<int>(10,20);  // Construct an Image of 10 rows, 20 clms
///      cout << i[1][1] << endl;        // Using brackets operations (RO)
///      i[1][1] = 7;                    // Using brackets operations (write)
/// \endcode
/// ///////////////////////////////////////////////////////////////////////
///     Yflipped img. For any DoublyBracketable
///
///     template <class DoublyBraketable>
///     struct ConstYflippedImg   ans struct YflippedImg
///          contains:  DoublyBraketable img
///
///     When operator[](rowNo) called , returns img[getHeight(img) - rowNo -1]
///
///     They can be built by global functions:
///
///     makeConstYflippedImg(img)
///          makeYflippedImg(img)
///
/// ///////////////////////////////////////////////////////////////////////
///     template < class DoublyBracketable , class OutPix>
///     void xFlipImg ( const DoublyBracketable& roi
///                       , Img<OutPix>* imgPtr)
///
///         Resize given img (pointed by imgPtr) and fill it by xFlipped roi
///
///
///     template <class DoublyBracketable>
///     Img<typename DoublyBracketable::Pixel> xFlipImg(const DoublyBracketable& roi)
///
///         similar function, returning Img<Pix>
/// ///////////////////////////////////////////////////////////////////////
///
///     template < class DoublyBracketable , class OutPix>
///     void yFlipImg ( const DoublyBracketable& roi
///                       , Img<OutPix>* imgPtr)
///
///         Resize given img (pointed by imgPtr) and fill it by yFlipped roi
///
///
///     template <class DoublyBracketable>
///     Img<typename DoublyBracketable::Pixel> yFlipImg(const DoublyBracketable& roi)
///
///         similar function, returning Img<Pix>
///
/// ///////////////////////////////////////////////////////////////////////
///     template < class DoublyBracketable , class OutPix>
///     void transposeImg ( const DoublyBracketable& roi
///                       , Img<OutPix>* imgPtr)
///
///         Resize given img (pointed by imgPtr) and fill it by TRANSPOSED roi
///
///
///     template <class DoublyBracketable>
///     Img<typename DoublyBracketable::Pixel> transposeImg(const DoublyBracketable& roi)
///
///         similar function, returning Img<Pix>
/// ///////////////////////////////////////////////////////////////////////
///     template < class DoublyBracketable >
///     bool img_isConst(const DoublyBracketable& roi)
///
/// =======================================================================================================

#ifndef IMG_H
#define IMG_H

#include <cassert>
#include <cctype>
#include <cstring>
#include <memory>
#include <limits>
#include <stdexcept>
#include <iterator>
#include <utility>

#include "Imgfwd.h"


//     class ImgBaseStorage
//
//     *---------------------------*
//     |   int*  intArray          |
//     *---------------------------*

class ImgBaseStorage {
    enum { minStorageLen_e = 4 };
    
    static int& privStaticCounter() {
        static int counter = 0;
        return counter;
    }
    
    static int privStorageLen() {
        int n = privStaticCounter();
        if (n < minStorageLen_e)
            n = minStorageLen_e;
        return n;
    }
    
    
    void privAllocateIntArray_ofzeros() {
        int n = privStorageLen();
        intArray = new int[n];
        memset(intArray , 0, n*sizeof(int));
    }
    
    static bool privUndefEqual2any(int f) {
        static bool imgRes0equal2everything = false;
        if (f != -1)
            imgRes0equal2everything = bool(f!=0);
        return imgRes0equal2everything;
    }
    
    int*    intArray;
    
public:
    ~ImgBaseStorage()      { delete[] intArray; intArray = nullptr;}
    
    ImgBaseStorage()
    : intArray(0)
    { }
    
    ImgBaseStorage(const ImgBaseStorage& other)
    : intArray(0)
    {
        if (other.intArray) {
            int n = privStorageLen();
            intArray = new int[n];
            memcpy(intArray , other.intArray, n*sizeof(int));
        }
    }
    
    ImgBaseStorage& operator=(const ImgBaseStorage& other) // TODO : redundant in #ifndef AA_NOT_SUPPORTED_RVALUE_REFERENCE???
    {
        if (this != &other) {
            this->~ImgBaseStorage();                // -yes. Kill old content of target.
            std::allocator<ImgBaseStorage>().construct(this ,other); // Run copy c-tor, using construct.
        }
        return *this;
    }
    
    
    /// ----------------- OF MOVE SEMANTICS -----------------------------
    
#ifndef AA_NOT_SUPPORTED_RVALUE_REFERENCE    /* i.e. VC++ 10.0 & higher supporting r-values (move semantics)*/
    
    // Implementation note:
    // If the copied object holds its own data, this data is copied. If it is pointing to an
    // externally allocated memory, the <this> object points to this same memory.
    ImgBaseStorage( ImgBaseStorage&& rValue)
    : intArray(rValue.intArray)
    {
        rValue.intArray = nullptr;
    }
    
    void operator=(ImgBaseStorage&& rValue)
    {
        if (this != &rValue) {
            this->~ImgBaseStorage();
            std::allocator<ImgBaseStorage>().construct(this, std::forward<ImgBaseStorage>(rValue));
            //            intArray = rValue.intArray;
            //            rValue.intArray = nullptr;
        }
    }
    
#endif
    
    /// ----------------- END OF MOVE SEMANTICS -----------------------------
    
    int  iword() const    { return intArray==nullptr ? 0 : privStorageLen();}
    
    int& iword(int index) {
        if (index >= privStorageLen())
            throw std::out_of_range("illegal index of Img::iword");
        if (intArray==nullptr)
            privAllocateIntArray_ofzeros();
        
        return intArray[index];
    }
    
    const int& iword(int index) const {
        const int& wordRef = const_cast<ImgBaseStorage*>(this)->iword(index);
        return wordRef;
    }
    
    static int xalloc() {
        int& counter = privStaticCounter();
        return counter++;
    }
    
    static int minStorageLen()     { return minStorageLen_e;}
    
    static bool isUndefImgResEqual2everything() { return privUndefEqual2any(-1);}
    
    static void isUndefImgResEqual2everything(bool f) {  privUndefEqual2any(int(f));}
    
    static bool isImgResDiffer(const ImgBaseStorage& a, const ImgBaseStorage& b)
    {
        if (a.iword()+b.iword()==0)
            return false;   // both of them are 0
        if(a.iword()*b.iword()==0)
            return isUndefImgResEqual2everything();    // 1 of them is 0 and other non 0
        if(a.iword(0)!=b.iword(0))
            return true;    // x-res differs
        if(a.iword(1)!=b.iword(1))
            return true;    // y-res differs
        
        return false;       // nothing differs
    }
};

template<class Pix>
inline int getHeight(const Img<Pix>& p);//    { return p.getHeight();}

template<class Pix>
inline int getWidth (const Img<Pix>& p);//    { return p.getWidth();}

template<class Pix>
inline int getBits  (const Img<Pix>& p);//    { Img<Pix>::base::PixelPtr q; return imgPixelBits(q);}

template <class Pix>
inline int __anyPixelBits(const Pix&) { return PixelTypes<Pix>::pixel_bits_e;}


template <class PixelPtr>
inline int ___defaultPixelIntensity()    { return 0;}

template <class PixelPtr, class Pix>
void ___rowset(PixelPtr row, int cols, Pix pix)   {
    
    // THE FOLLOWING LINES CAN BE UNCOMMENTED IN VC++2010
    typedef typename std::decay<decltype(*row)>::type  AnyPixel;
    static_assert(PixelTypes<Pix>::pixel_bits_e==PixelTypes<AnyPixel>::pixel_bits_e,"*PixPtr MUST have type Pix");
    
    for(int i=0;i!=cols;++i)
        row[i] = pix;
}

template <class PixelPtr, class AnyPixelPtr, class Pix>
void ___rowcpy(PixelPtr row, AnyPixelPtr from, int cols, Pix*, bool flipX)   {
    
    const void* voidPtrFrom = makeConstVoidPtrFrom(from);
    if (voidPtrFrom==nullptr)
        return ___rowset(row,cols,convert_to<Pix>::from(___defaultPixelIntensity<AnyPixelPtr>() ) );
    
    if(flipX) {
        for (int c=0; c!=cols; ++c)
            row[c] = convert_to<Pix>::from(from[cols -1 - c]);
    }
    else {
        for (int c=0; c!=cols; ++c)
            row[c] = convert_to<Pix>::from(from[c]);
    }
}


template <>
inline
void ___rowcpy ( PixelTypes<RGBpixel >::PixelPtr      row
                , PixelTypes<ARGBpixel>::ConstPixelPtr from, int cols, PixelTypes<RGBpixel >::PixelPtr, bool flipX)
{
    PixelTypes<RGBpixel >::PixelPtr rowend = row+cols;
    
    if(flipX) {
        from += cols-1;
        for (; row!=rowend; ++row,--from) {
            row->r = from->r;
            row->g = from->g;
            row->b = from->b;
        }
    }
    else {
        for (; row!=rowend; ++row,++from) {
            row->r = from->r;
            row->g = from->g;
            row->b = from->b;
        }
    }
    
}


template<class Pix>
class ImgBase : /*public ImgBaseStorage,*/ public ImgTypes<Pix>
{
    typedef   ImgTypes<Pix>       base;
    
public:
    typedef typename base::       value_type         value_type ;
    typedef typename base::       Pixel              Pixel      ;
    
    typedef typename base::ConstValue_typePtr   ConstValue_typePtr;
    typedef typename base::     Value_typePtr        Value_typePtr;
    
    typedef typename base::  ConstPixelPtr      ConstPixelPtr   ;
    typedef typename base::       PixelPtr           PixelPtr   ;
    typedef typename base::  ConstPixelRef      ConstPixelRef   ;
    typedef typename base::       PixelRef           PixelRef   ;
    typedef typename base::       FlatImg            FlatImg    ;
    
    typedef typename base::       PixBox             PixBox     ;
    
    enum {pixelsInBox_e = base::pixelsInBox_e };
    
    template<class S>   // S either float or double or ...
    static const S* dpmm(const S* xyDotsPerMMptr = nullptr)
    {
        static S xyDotsPerMM[2] = {72,72};
        if (xyDotsPerMMptr) {
            xyDotsPerMM[0] = xyDotsPerMMptr[0];
            xyDotsPerMM[1] = xyDotsPerMMptr[1];
        }
        return xyDotsPerMM;
    }
    
    
    
    static bool dbgMode(int onoff=-1) {
        static bool isDbgMode = false;
        if (onoff!=-1)
            isDbgMode = bool(onoff);
        return isDbgMode;
    }
    
    static void dbgReport(int kind, int onoff=-1)
    {
        static bool doprint = false;
        if (onoff!=-1)
            return void(doprint=bool(onoff!=0));
        
        if (!doprint)
            return;
        
        static const char* msgs[] = {
            "~ImgBase()"                      //  0
            , "~ImgBase(0)"                     //  1
            , "ImgBase()"                       //  2
            , "ImgBase(int,int,void*)"          //  3
            , "ImgBase(const ImgBase&)"         //  4
            , "ImgBase(ImgBase&&)"              //  5
            , "operator=(const ImgBase&)"       //  6
            , "operator=(ImgBase&&)"            //  7
        };
        std::cerr << std::endl << msgs[kind] << std::endl;
    }
    
    
    
    private :
    void allocImg_set_data()
    {
        data = (rows*cols==0 ? nullptr : new PixBox [rows*pixBoxesInRow()]);
    }
    
    void dealloc() {
        if(myBuf) {
            delete[] data;
            data = nullptr;
        }
        rows = cols = 0;    // make all data-members compatible
    }
    
public:
    ~ImgBase()        {  dbgReport(data ? 0 : 1); dealloc(); }
    
    ImgBase()  : /*ImgBaseStorage(),*/ rows(0) , cols(0), data(nullptr), myBuf(true)
    {  dbgReport(2);}
    
    ImgBase(size_t r, size_t c)   // c-tor
    : // ImgBaseStorage()
    rows(r), cols(c)
    , data(nullptr)
    , myBuf(true)
    {
        allocImg_set_data();
        dbgReport(3);
    }
    
    // memory already allocated by caller
    ImgBase(size_t r, size_t c, void* data)   // c-tor
    : // ImgBaseStorage()
    rows(r), cols(c)
    , data(reinterpret_cast<PixBox*>(data))
    , myBuf(false)
    {
        dbgReport(3);
    }
    
    // Implementation note:
    // If the copied object holds its own data, this data is copied. If it is pointing to an
    // externally allocated memory, the <this> object points to this same memory.
    ImgBase(const ImgBase& img)
    : // ImgBaseStorage(static_cast<const ImgBaseStorage&>(img))
    rows(img.rows), cols(img.cols)
    , data (img.data)
    , myBuf(img.myBuf)
    {
        if (myBuf)
        {
            allocImg_set_data();
            if (rows*cols!=0)
            {
                memcpy(data , img.data , rows*rowsize());
            }
        }
        dbgReport(4);
    }
    
    void operator=(const ImgBase& img)
    {
        if (&img != this) {
            this->~ImgBase();
            std::allocator<ImgBase<Pix> >().construct(this,img);
        }
        dbgReport(6);
    }
    
    /// -------------- MOVE SEMANTICS -------------------------
    
#ifndef AA_NOT_SUPPORTED_RVALUE_REFERENCE    /* i.e. VC++ 10.0 & higher supporting r-values (move semantics)*/
    
    // Implementation note:
    // If the copied object holds its own data, ptr to this data is copied.
    // and in the source object the ptr will be killed.
    ImgBase( ImgBase&& rhs)
    : // ImgBaseStorage(static_cast<const ImgBaseStorage&>(img))
    rows (rhs.rows), cols(rhs.cols)
    , data (rhs.data)     // just copy pointer
    , myBuf(rhs.myBuf)
    {
        // !!! temporary obj CAN be on user-buffer
        rhs.data = nullptr;
        rhs.rows = rhs.cols = 0;
        dbgReport(5);
    }
    
    void operator=(ImgBase&& rhs) // MOVE ASSIGNMENT OPERATOR
    {
        if (&rhs != this) {
            this->~ImgBase(); // as usual, free current resources
            std::allocator<ImgBase<Pix> >().construct(this,std::forward<ImgBase<Pix>>(rhs)); // will call ImgBase move c-tor thus setting rhs.myBuf to false
        }
        dbgReport(7);
    }
    
#endif
    
    /// ------------- END OF MOVE SEMANTICS ---------------------------
    
    void operator=(Pix pix)     // clear all img by pix
    {
        for (size_t i=0;i!=rows;++i)
            rowset(i , pix);
    }
    
    void complement() {
        for (size_t i=0;i!=rows;++i)
            rownot(i);
    }
    
    
    void resize(size_t r, size_t c)
    {
        if ((this->rows != r) || (this->cols != c)) {
            this->dealloc();
            this->rows = r;
            this->cols = c;
            if (myBuf)
                allocImg_set_data();
        }
    }
    
    
    PixelPtr operator [] (size_t i)        { assert(rowCk(i)); return makePixelPtr<     PixelPtr>(data+i*pixBoxesInRow());}
    ConstPixelPtr operator [] (size_t i) const  {
        assert(rowCk(i)); return makePixelPtr<ConstPixelPtr>(data+i*pixBoxesInRow());
    }
    
    int    getHeight () const                   { return int(rows);}
    int    getWidth  () const                   { return int(cols);}
    
    int    getBegClm() const                    { return 0;}
    int    getBegRow() const                    { return 0;}
    
    int    getEndClm() const                    { return int(cols);}
    int    getEndRow() const                    { return int(rows);}
    
    float  getStepClm() const                   { return 1.0f;}
    float  getStepRow() const                   { return 1.0f;}
    
    // get number of bytes in row
    int    rowsize() const                      { return pixBoxesInRow()*int(sizeof(PixBox));}
    
    template <class AnyPixelPtr>
    void rowcpy(size_t r, AnyPixelPtr from, bool flipX = false)   {
        
        //        const void* voidPtrFrom = (const void*)from;
        const void* voidPtrFrom = makeConstVoidPtrFrom(from);
        
        //         if (voidPtrFrom==0) {   // for zero-ptr just clean row by zeros
        //             rowset(r,Pix(0));
        //             return;
        //         }
        
        PixelPtr row = this->operator[](r);
        
        bool byteBound = (makePixelPtr<AnyPixelPtr>(voidPtrFrom)==from && !flipX);
        
        bool samePixSz = sizeof(*row)==sizeof(*from);
        
        int myBits = PixelTypes<Pix>::pixel_bits_e;
        
        // THE FOLLOWING LINES CAN BE UNCOMMENTED IN VC++2010
        //        typedef PixelTypeXby<AnyPixelPtr>::Pixel   AnyPixel;
        //        int frBits = PixelTypes<AnyPixel>::pixel_bits_e;
        //        bool canDo_memcpy = byteBound && voidPtrFrom!=nullptr && myBits==frBits;
        
        bool canDo_memcpy = byteBound && voidPtrFrom!=nullptr && myBits==__anyPixelBits(*from);
        
        if (canDo_memcpy)
            memcpy ( const_cast<void*>(makeConstVoidPtrFrom(row)) , voidPtrFrom, rowsize() );
        else
            ___rowcpy(row,from,int(cols),(Pix*)nullptr, flipX);
    }
    
    void rowset(size_t r, Pix pix)              { privSetRowPix(data+r*pixBoxesInRow() , pix);}
    
    void rownot(size_t r) {
        //    int nBoxesInRow = pixBoxesInRow();
        PixBox* begPtr = pixBoxRowPtr(r);
        //        PixBox* endPtr = pixBoxRowPtr(r+1);
        PixBox* endPtr = begPtr + pixBoxesInRow();
        for (; begPtr!=endPtr; ++begPtr) *begPtr = ~*begPtr;
    }
    
    
    template<class DoublyBracketable>
    bool operator==(const DoublyBracketable& img) const {
        bool equal = false;
        
        if (::getHeight(img) == rows && ::getWidth(img) == cols) {
            equal = true;
            for (size_t r=0; r!=rows && equal; ++r)
                if (! rowsRequal(this->operator [](r) , img[r] , this->cols))
                    equal = false;
        }
        return equal;
    }
    
    static bool rowsRequal(ConstPixelPtr a, ConstPixelPtr b, size_t n) {
        bool equal = true;
        for (;n;++a,++b,--n)
            if(!(*a == *b))
                break;
        return n==0;
    }
    
    
    template <class DoublyBracketable>
    void fillFrom(const DoublyBracketable& imgFrom , bool flipY = false, bool flipX = false)   {
        assert(rows == ::getHeight(imgFrom) && cols == ::getWidth (imgFrom));
        for (size_t r=0; r!=rows;++r)
            rowcpy(r,imgFrom[int(flipY ? (rows-1-r) : r)], flipX);
    }
    
    
    template <class DoublyBracketable>
    void cpyFrom(const DoublyBracketable& imgFrom , bool flipY = false, bool flipX = false)   {
        this->~ImgBase();
        rows = ::getHeight(imgFrom);
        cols = ::getWidth (imgFrom);
        allocImg_set_data();
        myBuf = true;   // so released by destructor
        fillFrom(imgFrom, flipY, flipX);
    }
    
    //    static size_t getRequiredPixBoxes(size_t rows,size_t cols) { return rows * privPixBoxesInRow(cols);}
    
    int pixBoxesInRow() const            { return privPixBoxesInRow(cols); }
    
    bool operator==(Pix pix) const       { return  isEqual(pix);}
    bool operator!=(Pix pix) const       { return !isEqual(pix);}
    
    bool isEqual(Pix pix) const       {
        for (size_t r=0; r!=rows; ++r)
            if (!isEqual(r,pix))
                return false;
        
        return true;
    }
    
    const PixBox* pixBoxRowPtr(size_t r) const  { return data + r*pixBoxesInRow();}
    PixBox* pixBoxRowPtr(size_t r)        { return data + r*pixBoxesInRow();}
    
    bool isEqual(size_t r, Pix pix) const
    {
        ConstPixelPtr rowPtr = operator[](r);
        return isEqual ( rowPtr , cols , pix);
    }
    
    
    static bool isEqual(ConstPixelPtr rowPtr, size_t nclms , Pix pix) {
        bool is0 = true;
        for (size_t i=0; i!=nclms; ++i)
            if (!(rowPtr[i]==pix)) {
                is0 = false;
                break;
            }
        return is0;
    }
    
protected:
    
    size_t    rows;
    size_t    cols;
    PixBox*   data;
    
    bool      myBuf;
    
private:
    
    static int privPixBoxesInRow(size_t cols) { return int(cols + base::pixelsInBox_e - 1)/pixelsInBox_e; }
    
    bool rowCk(size_t i) const     { return i < rows;}
    
    void privSetRowPix(PixBox* pixBoxPtr, Pix pix);
    
    void privSetRowChar(void* pixBoxPtr, char pix)
    {    // clear all img-row by pix
        memset(pixBoxPtr , pix , rowsize());
    }
    
};

// if given hint-data ptr is 0 ==> works exactly as ImgBase(rows,clms)
template<> inline
ImgBase<BoolTrue>::ImgBase(size_t rows, size_t cols)   // c-tor
: // ImgBaseStorage()
rows(rows), cols(cols)
, data(nullptr)
, myBuf(true)
{
    dbgReport(3);
}


template<> inline
ImgBase<BoolTrue>::ImgBase(const ImgBase<BoolTrue>& img)   // copy c-tor specialization
: // ImgBaseStorage()
rows(img.rows), cols(img.cols)
, data(nullptr)
, myBuf(true)
{
    dbgReport(3);
}


//template<>
//inline void ImgBase<BoolTrue>::allocImg_set_data () { /* for ImgBoolTrue do nothing*/}
//// ---------------------------------------------------------------------------------------------


// template<class Pix> inline
// void ImgBase<Pix>::rownot(size_t r)
// {
// //    int nBoxesInRow = pixBoxesInRow();
//     PixBox* begPtr = pixBoxRowPtr(r);
//     PixBox* endPtr = pixBoxRowPtr(r+1);
//     for (; begPtr!=endPtr; ++begPtr) *begPtr = ~*begPtr;
// }

template<class Pix> inline
void ImgBase<Pix>::privSetRowPix(PixBox* pixBoxPtr, Pix pix) {    // clear all img by pix
    bool sameChar = sizeof(Pix)==1 || memcmp(&pix , reinterpret_cast<char*>(&pix)+1, sizeof(Pix)-1)==0;
    if (sameChar)
        privSetRowChar(pixBoxPtr,*reinterpret_cast<char*>(&pix));
    else
        for (size_t i=0;i<cols;++i)
            pixBoxPtr[i] = pix;
}


template<> inline
void ImgBase<bool>::privSetRowPix(PixBox* pixBoxPtr, bool pix) {    // clear all img by pix
    int v = pix ? ~0 : 0;
    privSetRowChar(pixBoxPtr,v);
}

template<class Pix>
class Img : public ImgBase<Pix>
{
public:
    typedef   ImgBase<Pix>       base;
    
    typedef typename base::       value_type         value_type ;
    typedef typename base::       Pixel              Pixel      ;
    
    typedef typename base::ConstValue_typePtr   ConstValue_typePtr;
    typedef typename base::     Value_typePtr        Value_typePtr;
    
    typedef typename base::  ConstPixelPtr      ConstPixelPtr   ;
    typedef typename base::       PixelPtr           PixelPtr   ;
    typedef typename base::  ConstPixelRef      ConstPixelRef   ;
    typedef typename base::       PixelRef           PixelRef   ;
    typedef typename base::       FlatImg            FlatImg    ;
    
    ~Img()                      { }
    Img()                                      : ImgBase<Pix>()                 { }
    Img(size_t rows, size_t cols)              : ImgBase<Pix>(rows,cols)        { }
    Img(size_t rows, size_t cols,void* data)   : ImgBase<Pix>(rows,cols, data)  { }
    Img(const Img& img)                        : ImgBase<Pix>(img)              { }
    
    Img<Pixel>& operator=(const Img<Pixel>& img)           { if(this!=&img) ImgBase<Pixel>::operator=(img);return *this;}
    
    /// ------------ MOVE SEMANTICS ---------------------
    
#ifndef AA_NOT_SUPPORTED_RVALUE_REFERENCE    /* i.e. VC++ 10.0 & higher supporting r-values (move semantics)*/
    
    Img(Img&& rhs)
    : ImgBase<Pix>(std::move(rhs))
    { }
    
    Img<Pixel>& operator=(Img<Pixel>&& rhs)
    {
        assert(this!=&rhs);
        ImgBase<Pixel>::operator=(std::forward<ImgBase<Pix>>(rhs));
        return *this;
    }
#endif
    
    /// ------------ END OF MOVE SEMANTICS ---------------
    
    
    Img<Pixel>& operator=(const Pixel&  pix)                      { ImgBase<Pixel>::operator=(pix); return *this;}
    
    FlatImg*  getImg()         { return this;}
    const FlatImg*  getImg() const   { return this;}
    
    template <class Pix2>
    operator Img<Pix2>& () {
        priv_ckPixSz(Pix2());
        return *reinterpret_cast<Img<Pix2>*>((void*)(this));
    }
    
    template <class Pix2>
    operator const Img<Pix2>& () const {
        priv_ckPixSz(Pix2());
        return *reinterpret_cast<const Img<Pix2>*>((const void*)(this));
    }
    
private:
    template <class Pix2>
    void priv_ckPixSz(Pix2) const {
        static_assert(sizeof(Pix)==sizeof(Pix2) , "BAD cast images with different pixel size");
    }
};


template<class Pix>
inline int getHeight(const Img<Pix>& p)    { return p.getHeight();}

template<class Pix>
inline int getWidth (const Img<Pix>& p)    { return p.getWidth();}

template<class Pix>
inline int getBits  (const Img<Pix>& p)    {
    typedef typename Img<Pix>::PixelPtr PixelPtr;
    PixelPtr q;
    return imgPixelBits(q);
}

// -----------------------------------------------------
// BOOLEAN case i.e. Img<bool>
// -----------------------------------------------------


// specialization of ___rowset
template <>
inline void ___rowset(BitPtr row, int cols, bool pix)   {
    std::fill(row,row+cols,pix);
}

//
//// -----------------------------------------------------
//// NIBBLE case i.e. Img<NibblePixel>
//// -----------------------------------------------------
//
//template <class SignedUnsigned>
//struct NibRef {
//    typedef                   NibblePixel<SignedUnsigned>             Pixel;
//    typedef typename ImgTypes<NibblePixel<SignedUnsigned> >::PixBox   PixBox;
//
//    NibRef(PixBox* ptr, size_t idx)
//      : ptr(ptr + (idx>>1))
//      , upr(idx&1)
//      { }
//
//
//    operator Pixel() const         { return Pixel(getv());}
//
//    operator int() const           { return getv();}
//
//    void operator=(int v) {
//        v &= 0xF;
//        *ptr = upr ? ((*ptr & 0xF) + (v<<4)) : ((*ptr & 0xF0) + v);
//    }
//
//    void operator=(const NibRef& b) {
//        int v = b.operator int();
//        this->operator=(v);
//    }
//
//private:
//    PixBox*  ptr;
//    bool     upr;
//
//    int getv() const {
//        int v = *ptr;
//        if(upr)
//            v >>= 4;
//        return v & 0xF;
//    }
//};
//
//template <>
//inline
//NibRef<SignedPixel>::operator int() const {
//   int v = getv();
//   return v&8 ? (v|~7) : (v & 7);
//}
//
//template <class SignedUnsigned>
//struct NibPtr {
//    typedef   typename ImgTypes<NibblePixel<SignedUnsigned> >::PixBox   PixBox;
//    typedef   NibPtr<SignedUnsigned>                                    Self;
//    typedef   NibRef<SignedUnsigned>                                    NibRef;
//
//    PixBox* ptr;
//    size_t  idx;
//
//    NibPtr()
//        : ptr ()
//        , idx ()
//    {
//    }
//
//    explicit NibPtr(const void* p, size_t i=0)
//        : ptr ( reinterpret_cast<PixBox*>(const_cast<void*>(p)) + (i>>1) )
//        , idx ( i & 1 )
//    {
//    }
//
//    NibRef  operator*() const          { return NibRef(ptr,idx);}
//
//    NibRef  operator[](size_t i) const { return NibRef(ptr,idx+i);}
//
//    Self&   operator++()               { ++idx; return *this;}
//    Self    operator++(int)            { Self cur = *this; ++idx; return cur;}
//
//    Self&   operator--()               { --idx; return *this;}
//    Self    operator--(int)            { Self cur = *this; --idx; return cur;}
//
//    Self&   operator+=(int i)          { idx += i; return *this;}
//    Self    operator+ (int i) const    { return Self(ptr,idx+i);}
//
//    Self&   operator-=(int i)          { idx -= i; return *this;}
//    Self    operator- (int i) const    { return Self(ptr,idx-i);}
//
//    int     operator- (const Self& p) const { return (idx-p.idx) + 2*(ptr-p.ptr);}
//
//    bool    operator==(const Self& p) const
//    {
//        Self me =   reduce();
//        Self he = p.reduce();
//        return me.ptr == he.ptr && me.idx == he.idx;
//    }
//
//    bool    operator!=(const Self& p) const   { return !operator==(p);}
//
//    operator const void*() const  { const char* p = reinterpret_cast<char*>(ptr)+idx/2; return p;}
//    operator       void*()        {       char* p = reinterpret_cast<char*>(ptr)+idx/2; return p;}
//
//private:
//    Self reduce() const { return Self( ptr + (idx>>1) , idx & 1);}
//};
//
//////////////////////////////////////////////////////////////////////////

/// ----- specialization of Img<bool> -----------------------

template<>
class Img<bool> : public ImgBase<bool>
{
public:
    ~Img()                      { }
    Img()                                 : ImgBase<bool>()                 { }
    Img(size_t rows, size_t cols)               : ImgBase<bool>(rows,cols)        { }
    Img(size_t rows, size_t cols, void* data)   : ImgBase<bool>(rows,cols, data)  { }
    Img(const Img& img)                   : ImgBase<bool>(img)              { }
    
    void operator=(const Img& img)   { ImgBase<bool>::operator=(img);}
    
    /// ---------------- MOVE SEMANTICS ------------------
    
#ifndef AA_NOT_SUPPORTED_RVALUE_REFERENCE    /* i.e. VC++ 10.0 & higher supporting r-values (move semantics)*/
    
    Img(Img&& rhs)
    : ImgBase<bool>(std::forward<ImgBase<bool>>(rhs))
    { }
    
    Img<Pixel>& operator=(Img<Pixel>&& rhs)
    {
        assert(this!=&rhs);
        ImgBase<bool>::operator=(std::forward<ImgBase<bool>>(rhs));
        return *this;
    }
    
#endif
    
    /// ----------------- END OF MOVE SEMANTICS ------------
    
    
    void operator=(bool pix)         { ImgBase<bool>::operator=(pix);}
    
    FlatImg*  getImg()         { return this;}
    const FlatImg*  getImg() const   { return this;}
    
    static void reverseBytes ( void* ou , const void* in, size_t bits )    {
        typedef   unsigned char  uchar;
        size_t bytes = (bits+7)/8;
        uchar* pou = reinterpret_cast<      uchar*>(ou);
        const uchar* pin = reinterpret_cast<const uchar*>(in);
        uchar* ouend = pou + bytes;
        
        for (; pou!=ouend; ++pou, ++pin)
            *pou = reverseByte(*pin);
    }
    
    
    static int reverseByte ( unsigned int c )    {
        static unsigned char lut[256];
        if (lut[255] == 0)           // create 256 LUT if needed
            for (int i=0; i!=256; ++i)
                lut[i] = (privReverseNibble( i & 0xF ) << 4) + privReverseNibble( i >> 4 );
        
        return lut[c];
    }
    
private:
    
    static int privReverseNibble ( int c )    {
        assert (c<16);
        return (privReversePairBits(c&3) << 2) + privReversePairBits(c>>2);
    }
    
    static int privReversePairBits(int c) {
        assert (c < 4);
        return (c>>1) + (c&1)*2;
    }
    
};



template<>
class Img<BoolTrue> : public ImgBase<BoolTrue>
{
public:
    ~Img()                      { }
    Img()                                 : ImgBase<BoolTrue>()                 { }
    Img(size_t rows, size_t cols)               : ImgBase<BoolTrue>(rows,cols)        { }
    //    Img(size_t rows, size_t cols, void* data) : ImgBase<BoolTrue>(rows,cols, data)  { }
    
    //Img(const Img& img)                   : ImgBase<BoolTrue>(img)              { }
    
    //Img& operator=(const Img& img);  // generated by compiler
    
    FlatImg*  getImg()         { return this;}
    const FlatImg*  getImg() const   { return this;}
};
/// //////////////////////////////////////////////////////////////////////

template<class DoublyBracketable>
inline Img<typename DoublyBracketable::Pixel>   operator~(const DoublyBracketable& img)
{
    Img<typename DoublyBracketable::Pixel> res;
    res.cpyFrom(img);
    res.complement();
    return std::move(res);
}

inline ImgBool   operator~(const Img<BoolTrue>& img)    {
    ImgBool res(int(getHeight (img)), int(getWidth (img)));
    res = false;
    return std::move(res);
}

//////////////////////////////////////////////////////////////////////////
struct ImgBoopOpAnd {
    typedef ImgBool::PixBox    PixBox;
    void operator()(PixBox& a, PixBox b) { a &= b;}
};

struct ImgBoopOpOr {
    typedef ImgBool::PixBox    PixBox;
    void operator()(PixBox& a, PixBox b) { a |= b;}
};

struct ImgBoopOpXor {
    typedef ImgBool::PixBox    PixBox;
    void operator()(PixBox& a, PixBox b) { a ^= b;}
};

template<class AnyDB1,class AnyDB2>
inline void _ckEqsizes(const AnyDB1& a, const AnyDB2& b)
{
    int rows = int(getHeight(a));
    int clms = int(getWidth (a));
    bool sizes_of_2_pictures_are_equal = (rows==int(getHeight(b)) && clms==int(getWidth(b)));
    assert (sizes_of_2_pictures_are_equal);
    
    if (!sizes_of_2_pictures_are_equal)
        throw std::range_error("different sizes in logical op between ImgBool");
}

template <class Op>
void imgBoolOp(ImgBool& a , const ImgBool& b, Op op) {
    _ckEqsizes(a,b);
    int rows = getHeight(a);
    
    int nPixBoxes = a.pixBoxesInRow();
    
    typedef ImgBool::PixBox    PixBox;
    
    for (int r=0; r!=rows; ++r) {
        const PixBox* rowBpixBoxPtr = b.pixBoxRowPtr(r);
        PixBox* rowApixBoxPtr = a.pixBoxRowPtr(r);
        
        for (int i=0; i!=nPixBoxes; ++i)
            op(rowApixBoxPtr[i] , rowBpixBoxPtr[i]);
    }
}

//template <>
inline void imgBoolOp(ImgBool& a , const Img<BoolTrue>& b, ImgBoopOpAnd) { _ckEqsizes(a,b);}

//template <>
inline void imgBoolOp(ImgBool& a , const Img<BoolTrue>& b, ImgBoopOpOr ) { _ckEqsizes(a,b); a = true;}

//template <>
inline void imgBoolOp(ImgBool& a , const Img<BoolTrue>& b, ImgBoopOpXor) { _ckEqsizes(a,b); a = ~a;}

inline void operator &= (ImgBool& a , const ImgBool& b) { imgBoolOp(a,b,ImgBoopOpAnd());}
inline void operator |= (ImgBool& a , const ImgBool& b) { imgBoolOp(a,b,ImgBoopOpOr ());}
inline void operator ^= (ImgBool& a , const ImgBool& b) { imgBoolOp(a,b,ImgBoopOpXor());}

inline void operator &= (ImgBool& a , const Img<BoolTrue>& b) { imgBoolOp(a,b,ImgBoopOpAnd());}
inline void operator |= (ImgBool& a , const Img<BoolTrue>& b) { imgBoolOp(a,b,ImgBoopOpOr ());}
inline void operator ^= (ImgBool& a , const Img<BoolTrue>& b) { imgBoolOp(a,b,ImgBoopOpXor());}

//////////////////////////////////////////////////////////////////////////


// ---------------------------------------------------------------
// y-flipped img
// ---------------------------------------------------------------

template <class DoublyBracketable>
struct ConstYflippedImg : public ImgTypes < typename DoublyBracketable::Pixel > {
    typedef ImgTypes < typename DoublyBracketable::Pixel >      base;
    
    typedef  typename base::ConstPixelPtr    ConstPixelPtr;
    typedef  typename base::     PixelPtr         PixelPtr;
    
    typedef  typename base::ConstValue_typePtr   ConstValue_typePtr;
    typedef  typename base::     Value_typePtr        Value_typePtr;
    
    ConstYflippedImg(DoublyBracketable& img) : img(img)  { }
    
    ConstPixelPtr operator[](int row) const  { return img[getHeight(img)-1-row];}
    
    DoublyBracketable& img;
};


template <class DoublyBracketable>
struct YflippedImg : public ConstYflippedImg < DoublyBracketable > {
    
    typedef ConstYflippedImg < DoublyBracketable >      base;
    typedef  typename base::ConstPixelPtr    ConstPixelPtr;
    typedef  typename base::     PixelPtr         PixelPtr;
    
    YflippedImg(DoublyBracketable& img) : ConstYflippedImg<DoublyBracketable>(img)  { }
    
    ConstPixelPtr operator[](int row) const  { return ConstYflippedImg<DoublyBracketable>::operator[](row);}
    PixelPtr operator[](int row)        { return base::img[getHeight(base::img)-1-row];}
    
};


template<class DoublyBracketable>
inline int getHeight(const ConstYflippedImg<DoublyBracketable>& p)    { return getHeight(p.img);}

template<class DoublyBracketable>
inline int getWidth (const ConstYflippedImg<DoublyBracketable>& p)    { return int(getWidth (p.img));}

template<class DoublyBracketable>
inline int getBits  (const ConstYflippedImg<DoublyBracketable>& p)    { return int(getBits  (p.img));}

template <class DoublyBracketable>
ConstYflippedImg<DoublyBracketable> makeConstYflippedImg(DoublyBracketable& img)  {
    return ConstYflippedImg<DoublyBracketable>(img);
}

template <class DoublyBracketable>
YflippedImg<DoublyBracketable> makeYflippedImg(DoublyBracketable& img)  {
    return YflippedImg<DoublyBracketable>(img);
}

//////////////////////////////////////////////////////////////////////////
///  Img<typename DoublyBracketable::Pixel> xFlipImg(const DoublyBracketable& roi)
//////////////////////////////////////////////////////////////////////////

template < class DoublyBracketable , class OutPix>
void xFlipImg ( const DoublyBracketable& roi, Img<OutPix>* imgPtr)
{
    imgPtr->cpyFrom(roi,false, true);  // flipY = false , flipX = true
    
}

template <class DoublyBracketable>
Img<typename DoublyBracketable::Pixel> xFlipImg(const DoublyBracketable& roi)
{
    typedef typename DoublyBracketable::Pixel  Pix;
    Img<Pix> outImg;
    xFlipImg ( roi , &outImg);
    return std::move(outImg);
}

//////////////////////////////////////////////////////////////////////////
///  Img<typename DoublyBracketable::Pixel> yFlipImg(const DoublyBracketable& roi)
//////////////////////////////////////////////////////////////////////////

template < class DoublyBracketable , class OutPix>
void yFlipImg ( const DoublyBracketable& roi, Img<OutPix>* imgPtr)
{
    imgPtr->cpyFrom(roi, true);    // flipY = true , flipX = false (default)
}

template <class DoublyBracketable>
Img<typename DoublyBracketable::Pixel> yFlipImg(const DoublyBracketable& roi)
{
    typedef typename DoublyBracketable::Pixel  Pix;
    Img<Pix> outImg;
    yFlipImg ( roi , &outImg);
    return std::move (outImg);
}

//////////////////////////////////////////////////////////////////////////
///  Img<typename DoublyBracketable::Pixel> transposeImg(const DoublyBracketable& roi)
//////////////////////////////////////////////////////////////////////////

template <class DoublyBracketable , class OutImg>
void transposeImg(const DoublyBracketable& roi , OutImg* imgPtr)  {
    
    int rows = getHeight(roi);
    int clms = getWidth (roi);
    
    imgPtr->resize(clms,rows);
    OutImg& img = *imgPtr;
    
    if (clms < rows) {  // i.e. clm is HIGH
        typedef typename OutImg::PixelPtr  PixelPtr;
        for (int clmNo=0; clmNo!=clms; ++clmNo) {
            PixelPtr ouRow = img[clmNo];
            for (int rowNo=0; rowNo!=rows; ++rowNo)
                ouRow[rowNo] = roi[rowNo][clmNo];
        }
    }else {     // i.e. row  is LONG
        typedef typename DoublyBracketable::ConstPixelPtr  ConstPixelPtr;
        
        for (int rowNo=0; rowNo!=rows; ++rowNo) {
            ConstPixelPtr inRow = roi[rowNo];
            for (int clmNo=0; clmNo!=clms; ++clmNo)
                img[clmNo][rowNo] = inRow[clmNo];
        }
    }
    
}


//template <>
inline void transposeImg(const Img<BoolTrue>& img , Img<BoolTrue>* imgPtr)  {
    
    int rows = int(getHeight(img));
    int clms = int(getWidth (img));
    
    *imgPtr = ImgBoolTrue(clms,rows);
}


template <class DoublyBracketable>
typename DoublyBracketable::FlatImg transposeImg(const DoublyBracketable& roi)
//Img<typename DoublyBracketable::Pixel> transposeImg(const DoublyBracketable& roi)
{
    typedef typename DoublyBracketable::Pixel  Pix;
    Img<Pix> outImg;
    transposeImg ( roi , &outImg);
    return std::move (outImg);
}

/// ///////////////////////////////////////////////////////////////////////////

class _ImgRasterRowChecker {
    int m_clms;
public:
    _ImgRasterRowChecker(int clms) : m_clms(clms)    {}
    
    template <class ConstPixPtr>
    bool operator()(ConstPixPtr rowPtr) const {
        if (m_clms==0)
            return true;
        int c;
        for (c=1; c!=m_clms; ++c)
            if(!(rowPtr[c] == rowPtr[0]))
                break;
        bool yes = (c==m_clms);
        return yes;
    }
};

template <>   // fast specification for BitPtr
inline
bool _ImgRasterRowChecker::operator()(BitPtr bitrow) const {
    if (m_clms==0)
        return true;
    int v = bitrow[0];
    //    BitPtr p = bitrow.findfirst( (1-v)!=0 , clms);
    BitPtr p = std::find(bitrow, bitrow+m_clms , (1-v)!=0 );
    
    bool yes = ((p-bitrow) == m_clms);
    return yes;
}



class _ImgCompressedRowChecker {
    int clms;
public:
    _ImgCompressedRowChecker(int clms) : clms(clms)    {}
    
    template <class T>
    bool operator()(const T* runs) const {
        if (runs[1]==0 || (runs[0]==0 && (runs[1]==0 || runs[2]==0))) {
            // either runs[1] is terminating run(black row)
            // or runs[0]==0(white row) && (runs[1] || runs[2] is terminating run)
            assert(int(runs[0]+runs[1]) == clms);
            return true;
        }
        return false;
    }
};

///////////////////////////////////

template <class ArrayImgRows, class RowChecker>
inline
bool _img_isConst(const ArrayImgRows& imgRows, RowChecker rowChecher )
{
    //  class RowChecker must have operator()(imgRowPtr) const which returns 'bool'
    int rows = int(getHeight(imgRows));
    int clms = int(getWidth (imgRows));
    
    if (rows*clms==0)
        return true;
    
    typedef typename ArrayImgRows::ConstPixelPtr  ConstPixelPtr;
    ConstPixelPtr row0ptr = imgRows[0];
    
    int r;
    for (r=0; r!=rows; ++r) {
        ConstPixelPtr rowRptr = imgRows[r];
        if (!rowChecher(rowRptr))
            break;
        if (!(rowRptr[0]==row0ptr[0]))
            break;
    }
    
    bool isConst = (r==rows);
    return isConst;
}


template<bool runRows> struct _Img_isConst; // false means raster rows, true means compressed rows

// specification : raster rows
template <>
struct _Img_isConst<false> {
    template < class ArrayImgRows>
    bool img_isConst(const ArrayImgRows& roi) const  { return _img_isConst(roi,_ImgRasterRowChecker(getWidth(roi)));}
};

// specification : compressed rows
template <>
struct _Img_isConst<true> {
    template < class ArrayImgRows>
    bool img_isConst(const ArrayImgRows& runsImg) const  { return _img_isConst(runsImg,_ImgCompressedRowChecker(getWidth(runsImg)));}
};


template < class DoublyBracketable >
inline
bool img_isConst(const DoublyBracketable& roi) { return _Img_isConst<false>().img_isConst(roi);}


template <>
inline
bool img_isConst(const ImgBoolTrue&) { return true;}


#endif

