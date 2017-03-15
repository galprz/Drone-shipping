/// \file Imgfwd.h \date 1-JAN-2010 \author Haim Kreitman & Chanan Gazala                                                                           \n
/// \brief This file contains forward declarations for Img types                                                                                    \n
/// \details Description:                                                                                                                           \n
/// ================                                                                                                                                \n
///    Imgfwd.h - This file contains forward declarations for Img types :                                                                           \n
///    -------------------------------------------------------------                                                                                \n
///    Predefined typedefs:                                                                                                                         \n
/// --------------------------------------------------------                                                                                        \n
///                                                                                                                                                 \n
///          WRPpixelT<unsigned      char >   BWRPpixel;                                                                                            \n
///          WRPpixelT<  signed      char >   SWRPpixel;                                                                                            \n
///          WRPpixelT<              short>   HWRPpixel;                                                                                            \n
///          WRPpixelT<unsigned      short>   WWRPpixel;                                                                                            \n
///          WRPpixelT<              int  >   IWRPpixel;                                                                                            \n
///          WRPpixelT<unsigned      int  >   UWRPpixel;                                                                                            \n
///          WRPpixelT<              long >   LWRPpixel;                                                                                            \n
///          WRPpixelT<unsigned      long >   GWRPpixel;                                                                                            \n
///          WRPpixelT<         long long >   KWRPpixel;                                                                                            \n
///          WRPpixelT<unsigned long long >   QWRPpixel;                                                                                            \n
///          WRPpixelT<              float>   FWRPpixel;                                                                                            \n
///          WRPpixelT<             double>   DWRPpixel;                                                                                            \n
///                                                                                                                                                 \n
///          RGBpixelT<unsigned      char >    RGBpixel;                                                                                            \n
///          RGBpixelT<  signed      char >   SRGBpixel;                                                                                            \n
///          RGBpixelT<              short>   HRGBpixel;                                                                                            \n
///          RGBpixelT<unsigned      short>   WRGBpixel;                                                                                            \n
///          RGBpixelT<              int  >   IRGBpixel;                                                                                            \n
///          RGBpixelT<unsigned      int  >   URGBpixel;                                                                                            \n
///          RGBpixelT<              long >   LRGBpixel;                                                                                            \n
///          RGBpixelT<unsigned      long >   GRGBpixel;                                                                                            \n
///          RGBpixelT<         long long >   KRGBpixel;                                                                                            \n
///          RGBpixelT<unsigned long long >   QRGBpixel;                                                                                            \n
///          RGBpixelT<              float>   FRGBpixel;                                                                                            \n
///          RGBpixelT<             double>   DRGBpixel;                                                                                            \n
///                                                                                                                                                 \n
///                                                                                                                                                 \n
///   And also appropriate Img types:                                                                                                               \n
/// ----------------------------------------------------------------------                                                                          \n
///       Img<  signed  char>        ImgChar;  // 1 byte                                                                                            \n
///       Img<unsigned  char>        ImgByte;  // 1 byte                                                                                            \n
///       Img<unsigned short>        ImgWord;  // 2 bytes unsigned short                                                                            \n
///       Img<bool>                  ImgBool;  // 1 bit                                                                                             \n
///       Img<int>                   ImgLong;  // 4 bytes int                                                                                       \n
///       Img<unsigned int>          ImgDwrd;  // 4 bytes unsigned int (Double WoRD)                                                                \n
///       Img<long long>             ImgKwrd;  // 8 bytes long long    (Quadro WoRD)                                                                \n
///       Img<unsigned long long>    ImgQwrd;  // 8 bytes long long    (Quadro WoRD)                                                                \n
///       Img<float>                 ImgReal;  // 4 bytes float                                                                                     \n
///       Img<double>                ImgDblp;  // 8 bytes DouBLe Precision                                                                          \n
///       Img<ARGBpixel>             ImgARGB;  //32  bit (4 bytes)                                                                                  \n
///       Img< RGBpixel>             Img_RGB;  //24  bit (3 bytes)                                                                                  \n
///       Img<WRGBpixel>             ImgWRGB;  //48  bit (3 words)                                                                                  \n
///       Img<IRGBpixel>             ImgIRGB;  //96  bit (3 ints)                                                                                   \n
///       Img<FRGBpixel>             ImgFRGB;  //96  bit (3 floats)                                                                                 \n
///       Img<DRGBpixel>             ImgDRGB;  //192 bit (3 doubles)                                                                                \n
///                                                                                                                                                 \n
///           Typedefs : value_type (for pixel)                                                                                                     \n
///                      Pixel      (the same)                                                                                                      \n
///                      ConstPixelPtr                                                                                                              \n
///                      PixelPtr                                                                                                                   \n
///                      ConstPixelRef                                                                                                              \n
///                      PixelRef                                                                                                                   \n
///                      FlatImg                                                                                                                    \n
///                                                                                                                                                 \n
///           Example:   ImgByte::PixelPtr   is   unsigned char*                                                                                    \n
///                      ImgBool::PixelPtr   is   BitPtr                                                                                            \n
///                                                                                                                                                 \n
/// -------------------------------------------------------------------------------                                                                 \n
/// !!! ------- USER DEFINED PIXEL -----------------                                                                                                \n
/// ///////////////////////////////////////////////////////////////////////                                                                         \n
///  Bit pointers for rows of ImgBool                                                                                                               \n
///                                                                                                                                                 \n
///  BitPtr is ImgBool::PixelPtr                                                                                                                    \n
///                                                                                                                                                 \n
///  BitPtr(const void* p)                                                                                                                          \n
///                                                                                                                                                 \n
///  All pointer arithmetic is available                                                                                                            \n
///                                                                                                                                                 \n
///  Can be converted to  char* (and const char*) ignoring obsolete bits                                                                            \n
///                                                                                                                                                 \n
///  global-functions:                                                                                                                              \n
///  -----------------                                                                                                                              \n
///  BitPtr std::find(BitPtr frst, BitPtr last, const bool& v)                                                                                      \n
///  void   std::fill(BitPtr frst, BitPtr last, const bool& v)                                                                                      \n
///                                                                                                                                                 \n
///                                                                                                                                                 \n
///  // pack pointed bits to runs, returning # of runs (excluding terminating 0-run)                                                                \n
///  // 1st run is always BLACK, i.e. color 0, and ONLY 1-st run can have 0 length                                                                  \n
///  // (iff 1st pointed bit is 1). The last terminating 0-run, always added.                                                                       \n
///  template < class OutIterator >  // *OutIterator should be any integral type (int,uchar,uint,schar,etc...)                                      \n
///  int pack2runs(BitPtr from, OutIterator rlePtr , size_t clms)                                                                                   \n
///                                                                                                                                                 \n
///  /// pack pointed bits to runs, returning # of runs (excluding terminating zero-run).                                                           \n
///  /// 1st run is always BLACK, i.e. color 0, and ONLY 1st run can have 0 length                                                                  \n
///  /// (iff 1st pointed bit is 1).                                                                                                                \n
///  /// If nRuns NOT given ==> work till zero run (but always after rlePtr[0])                                                                     \n
///  template < class InputIterator > // *InputIterator should be any integral type (int,uchar,uint,schar,etc...)                                   \n
///  int runs2bits(InputIterator rlePtr , BitPtr into, size_t nRuns = ~0)                                                                           \n
///                                                                                                                                                 \n
///  /// convert buffer of ANY pixels to BINARY pixels, looking on INPUT pixels                                                                     \n
///  /// as ZEROS and NON-ZEROS                                                                                                                     \n
///  template < class PixConstPtr >                                                                                                                 \n
///  void packFrom(PixConstPtr pixRowConstPtr, BitPtr into, size_t clms)                                                                            \n
///                                                                                                                                                 \n
///                                                                                                                                                 \n
/// -------------------------------------------------------------------------------                                                                 \n
///                                                                                                                                                 \n
///  for usage of the <algorithm> library, the following iterators are supplied:                                                                    \n
///  For X-direction just ConstPixelPtr and PixelPtr can be used.                                                                                   \n
///  For Y-direction, use the following:                                                                                                            \n
///                                                                                                                                                 \n
///  template<AnyPixelPtr> class      Yiterator                                                                                                     \n
///  template<AnyPixelPtr> class ConstYiterator                                                                                                     \n
///                                                                                                                                                 \n
///  2 adapter functions are supplied:                                                                                                              \n
///                                                                                                                                                 \n
///  Yiterator<typename AnyDB::PixelPtr>           clmIter = make_Yiterator     (roi, 10); //       random_access_iterator for clm 10 on AnyDB roi  \n
///  ConstYiterator<typename AnyDB::ConstPixelPtr> clmIter = make_ConstYiterator(roi, 10); // const random_access_iterator for clm 10 on AnyDB roi  \n
/// -------------------------------------------------------------------------------                                                                 \n
///                                                                                                                                                 \n
///  for the trivial 'X-iterators' 2 adapter functions also supplied:                                                                               \n
///                                                                                                                                                 \n
///  ConstPixelPtr       clmIter = make_Xiterator     (roi, 10); //       random_access_iterator for row 10 on AnyDB roi                            \n
///       PixelPtr       clmIter = make_ConstXiterator(roi, 10); // const random_access_iterator for row 10 on AnyDB roi                            \n
/// ///////////////////////////////////////////////////////////////////////                                                                         \n

#ifndef IMGFWD_H
#define IMGFWD_H

#include <cassert>
#include <cmath>
#include <cstddef>              // to get ptrdiff_t
#include <utility>              // to get rel_ops
#include <limits>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <iterator>
#include <type_traits>

/// FORWARD DECLARATIONS

//#define VECTOR_ITERATOR_MODE

#ifndef VECTOR_ITERATOR_MODE
struct BitPtr;
#else
#ifndef DECLARE_BITPTR
#define DECLARE_BITPTR
#include <vector>     // to get vector<bool>::iterator
typedef std::vector<bool>::iterator  BitPtr;
//    typedef std::vector<bool>::reference BitRef;
#endif

#endif

typedef unsigned int        BitPtr_PixBox;


/// ----- now declare BitRef and BitPtr types ------------------
#ifndef VECTOR_ITERATOR_MODE

struct BitRef_ {
    //    typedef    PixelTypesHelp<bool>::PixBox     PixBox;
    typedef    BitPtr_PixBox          PixBox;
    enum {log2boxBytes_e = sizeof(PixBox)/2-sizeof(PixBox)/8};
    // for 1(char),2(short),4(int),8(__int64) we'll have 0,1,2,3
    enum {log2boxBits_e = log2boxBytes_e+3};
    // for 1(char),2(short),4(int),8(__int64) we'll have 3,4,5,6
    enum {idxMask_e = (1<<log2boxBits_e)-1};
    // for 1(char),2(short),4(int),8(__int64) we'll have 0x7,0xF,0x1F,0x3F
    
    BitRef_()
    : ptr(0)
    , bit(0)
    { }
    
    BitRef_(PixBox* ptr, size_t idx)
    : ptr(ptr+(idx>>log2boxBits_e))
    , bit(1 << (idx&idxMask_e))
    { }
    
    operator bool() const      { return ((*ptr & bit) != 0);}
    
    template<class T>
    void operator=(T v) {
        if(v!=T(0)) *ptr |=  bit;
        else        *ptr &= ~bit;
    }
    
    void operator=(bool v) {
        if(v) *ptr |=  bit;
        else  *ptr &= ~bit;
    }
    
    void operator=(const BitRef_& b) {
        if (*b.ptr & b.bit) *ptr |=  bit;
        else                *ptr &= ~bit;
    }
    
private:
    PixBox*  ptr;
    int      bit;
};


struct BitPtr : public std::iterator<std::random_access_iterator_tag, bool, ptrdiff_t , BitPtr, BitRef_>
{
    typedef    BitRef_            reference;
    typedef    difference_type    DistType;
    typedef    BitRef_::PixBox    PixBox;
    
    enum {boxBits_e = sizeof(PixBox)*8};   // 32
    enum {idxMask_e = BitRef_::idxMask_e}; // 0x1F for PixBox==unsigned int
    enum {ptrMask_e = (idxMask_e >> 3) };  // 0x03 for PixBox==unsigned int
    // for 1(char),2(short),4(int),8(__int64) we'll have 0x0,0x1,0x3 ,0x7
    
    PixBox* ptr;
    size_t  idx;
    
    BitPtr()
    : ptr (0)
    , idx (0)
    { }
    
    BitPtr(const BitPtr& p)
    : ptr (p.ptr)
    , idx (p.idx)
    {
        assert ((idx & ~idxMask_e)==0);
    }
    
    BitRef_ operator*() const          { return BitRef_(ptr,idx);}
    BitRef_ operator[](size_t i) const { return BitRef_(ptr,idx+i);}
    
    BitPtr& operator++()               { if(++idx==boxBits_e) ++ptr,(idx=0); return *this;}
    BitPtr  operator++(int)            { BitPtr cur = *this; this->operator++(); return cur;}
    
    BitPtr& operator--()               { if(idx==0) --ptr,(idx=boxBits_e); --idx; return *this;}
    BitPtr  operator--(int)            { BitPtr cur = *this; this->operator--(); return cur;}
    
    BitPtr  operator+ (DistType i) const { BitPtr cur = *this; return cur += i;}
    BitPtr& operator+=(DistType i)       {
        idx += i;
        if ((idx&~idxMask_e)!=0) {
            ptr += DistType(idx&~idxMask_e)/boxBits_e;
            idx &= idxMask_e;
        }
        return *this;
    }
    
    BitPtr  operator- (DistType i) const { BitPtr cur = *this; return cur += -i;}
    BitPtr& operator-=(DistType i)       { return this->operator +=(-i);}
    
    DistType operator- (const BitPtr& p) const {
        DistType bitDist = DistType(ptr-p.ptr)*sizeof(PixBox)*8;
        assert((bitDist>>3)==(ptr-p.ptr)*sizeof(PixBox));
        return int(idx-p.idx) + bitDist;
    }
    
    bool    operator==(const BitPtr& p) const    {
        return ptr == p.ptr && idx == p.idx;
    }
    
    bool    operator!=(const BitPtr& p) const   { return !operator==(p);}
    
    bool    operator< (const BitPtr& p) const    {
        return ptr < p.ptr || ptr == p.ptr && idx < p.idx;
    }
};

#endif

// -------------------------------------------------------
#ifndef MAKE_PIXEL_PTR
#define MAKE_PIXEL_PTR
#include <type_traits>
#include <iterator>

template <class Tptr> inline Tptr _makePixelPtrHelper(const void* p, std::true_type ) { return reinterpret_cast<Tptr>(const_cast<void*>(p));}
template <class Tptr> inline Tptr _makePixelPtrHelper(const void* p, std::false_type) {
    static Tptr zeroPtr;    // guaranteed all bits are zeros: actually Tptr here can be only BitPtr/std::vector<bool>::iterator
    return zeroPtr+reinterpret_cast<typename std::iterator_traits<Tptr>::difference_type>(p)*8;
}

template <class Tptr> inline Tptr makePixelPtr(const void* p) {
    typename std::is_pointer<Tptr>::type   v;
    // v is variable of type std::true_type of std::false_type
    return _makePixelPtrHelper<Tptr>(p, v);
}

#endif
// -------------------------------------------------------


struct BoolTruePtr {
    BoolTruePtr()                     { }
    explicit BoolTruePtr(const void*) { }
    bool operator[](size_t i) const   { return true;}
    operator const void*() const      { return 0;   }
    operator       void*()            { return 0;   }
    bool operator*() const            { return true;}
};

// exclusively  defined here (out of ifndef-endif)
template <>        inline BoolTruePtr makePixelPtr(const void* p) { return BoolTruePtr();}

// -------------------------------------------------------
#ifndef MAKE_CONST_VOID_PTR_FROM
#define MAKE_CONST_VOID_PTR_FROM
#include <type_traits>

template <class Tptr> inline const void* _makeConstVoidPtrHelper(Tptr p, std::true_type ) {  return p; }

template <class Tptr> inline const void* _makeConstVoidPtrHelper(Tptr p, std::false_type) {
    static Tptr zeroPtr;    // guaranteed all bits are zeros: actually Tptr here can be only BitPtr/std::vector<bool>::iterator
    return reinterpret_cast<void*> ((p - zeroPtr)/8);
}

template <class Tptr> inline const void* makeConstVoidPtrFrom(Tptr p) {
    typename std::is_pointer<Tptr>::type   v;
    // v is variable of type std::true_type of std::false_type
    return _makeConstVoidPtrHelper(p,v);
}

#endif
// -------------------------------------------------------

inline BitPtr_PixBox* getPixBoxPtr_idx(BitPtr p, int* idx=0)
{
    typedef BitPtr_PixBox    PixBox;
    enum {boxBits_e = sizeof(PixBox)*8};   // 32
    
    BitPtr::difference_type bitNo = p - BitPtr();
    
    PixBox* ptr = reinterpret_cast<PixBox*>((bitNo/boxBits_e)*sizeof(PixBox));
    
    if (idx)
        *idx = int(bitNo % boxBits_e);
    
    return ptr;
}

// -------------------------------------------------------

typedef BitPtr::reference   BitRef;

struct BoolTrue {};

/// --------------------------------------------------------------------
//template<class X> struct ImgfwdIteratorDeref     { typedef typename X::value_type type;};
//template<class X> struct ImgfwdIteratorDeref<X*> { typedef          X             type;};
/// --------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
/// pack pointed bits to runs, returning # of runs (excluding terminating 0-run)
/// 1st run is always BLACK, i.e. color 0, and ONLY 1-st run can have 0 length
//  (iff 1st pointed bit is 1). The last terminating 0-run, always added.
///////////////////////////////////////////////////////////////////////////////
template < class OutItor >   // R should be any integral type (int,uchar,uint,schar,etc...)
int pack2runs( BoolTruePtr from,OutItor rlePtr , int clms)
{
    //        typedef typename std::iterator_traits<OutItor>::value_type Rr;
    //        typedef decltype(*rlePtr) R;
    //const char* rname = typeid(R).name();  // IN 2010 can be void for back_insert_iterator
    
    *rlePtr   = 0;      // starting zero-BLACK run
    *++rlePtr = clms;   // WHITE run
    *++rlePtr = 0;      // terminating zero-run
    return 2;
}


//template<>
inline int imgPixelBits(const BoolTruePtr& p)    { return 1;}

inline  BoolTruePtr operator+(const BoolTruePtr& p, int i)   { return p;}

inline  BoolTruePtr operator-(const BoolTruePtr& p, int i)   { return p;}

inline ptrdiff_t    operator-(const BoolTruePtr&, const BoolTruePtr&) { return ptrdiff_t(0);}

/// ///////////////////////////////////////////////////////////////////////

struct BoolTrueRef {
    //     typedef    PixelTypesHelp<bool>::PixBox     PixBox;
    //     enum {log2boxBytes_e = sizeof(PixBox)/2-sizeof(PixBox)/8};
    //     // for 1(char),2(short),4(int),8(__int64) we'll have 0,1,2,3
    //     enum {log2boxBits_e = log2boxBytes_e+3};
    //     // for 1(char),2(short),4(int),8(__int64) we'll have 3,4,5,6
    //     enum {idxMask_e = (1<<log2boxBits_e)-1};
    //     // for 1(char),2(short),4(int),8(__int64) we'll have 0x7,0xF,0x1F,0x3F
    
    BoolTrueRef()              { }
    
    operator bool() const      { return true;}
    
    //    bool operator==(const BoolTrueRef& br) const { return true;}
};

/// PIXEL REFERENCE TYPES & QUANTITY ENUMERATORS (i.e. number of digits)

template <class Pix>
struct PixelTypesHelp {
    typedef const Pix*  ConstValue_typePtr;
    typedef       Pix*       Value_typePtr;
    
    typedef const Pix*  ConstPixelPtr;
    typedef       Pix*       PixelPtr;
    typedef const Pix&  ConstPixelRef;
    typedef       Pix&       PixelRef;
    
    typedef Pix              PixBox;
    //--// PixBox normally contains 1 pixel ( in bool   case - 32 pixels
    //--//                                  , in nibble case -  2 pixels)
    
    enum { pixelsInBox_e = 1 };
    enum { pixel_bits_e = int(sizeof(Pix)*std::numeric_limits<unsigned char>::digits)};
};

template <>
struct PixelTypesHelp<bool> {
    typedef BitPtr  ConstValue_typePtr;
    typedef BitPtr       Value_typePtr;
    
    typedef BitPtr             ConstPixelPtr;
    typedef BitPtr                  PixelPtr;
    typedef BitPtr::reference  ConstPixelRef;
    typedef BitPtr::reference       PixelRef;
    
    typedef unsigned int PixBox;
    //--// PixBox normally contains 1 pixel ( in bool   case - 32 pixels
    //--//                                  , in nibble case -  2 pixels)
    
    enum { pixelsInBox_e = std::numeric_limits<PixBox>::digits };
    enum { pixel_bits_e = 1};
};

template <>
struct PixelTypesHelp<BoolTrue> {
    typedef BoolTruePtr  ConstValue_typePtr;
    typedef BoolTruePtr       Value_typePtr;
    
    typedef BoolTruePtr  ConstPixelPtr;
    typedef BoolTruePtr       PixelPtr;
    
    typedef BoolTrueRef  ConstPixelRef;
    typedef BoolTrueRef       PixelRef;
    
    typedef unsigned int    PixBox;
    //--// PixBox normally contains 1 pixel ( in bool   case - 32 pixels
    //--//                                  , in nibble case -  2 pixels)
    
    enum { pixelsInBox_e = std::numeric_limits<PixBox>::digits };
    enum { pixel_bits_e = 1};
};


template <class T>   // T is either unsigned int or char
void fastForwardSameClr ( int runClr, T* rowPtr , int clms, int& counter, int& c, size_t ofs)
{
    int bits = sizeof(T)*8;
    if ((c+ofs)%bits!=0)      // is it T bound ?
        return;
    
    while(c+bits < clms) {
        T w = rowPtr[(c+ofs)/bits];
        bool all0s_or_1s = (runClr==0 &&  w==T(0)
                            || runClr!=0 && ~w==T(0) );
        if (!all0s_or_1s)
            break;
        
        c       += bits;
        counter += bits;
    }
}


inline int first1from(BitPtr_PixBox box)
{
    typedef BitPtr_PixBox    PixBox;
    
    if (box==0)
        return sizeof(PixBox)*8;    // 32
    
    // ---- find #of 1 inside 'bit'
    int bitNo = 0;
    PixBox singleBit = (((box-1)^box) >> 1)+1;
    
    for(int nBits = sizeof(PixBox)*8/2; nBits>0; nBits /= 2) {
        if ((singleBit&((1<<nBits)-1))!=0)
            continue;
        bitNo += nBits;
        singleBit >>= nBits;
    }
    return bitNo;
}



///////////////////////////////////////////////////////////////////////////////
/// pack pointed bits to runs, returning # of runs (excluding terminating 0-run)
/// 1st run is always BLACK, i.e. color 0, and ONLY 1-st run can have 0 length
//  (iff 1st pointed bit is 1). The last terminating 0-run, always added.
///////////////////////////////////////////////////////////////////////////////
template < class OutItor >   // *OutItor should be any integral type (int,uchar,uint,schar,etc...)
inline
int pack2runs( BitPtr from, OutItor rlePtr , size_t _clms)
{
    typedef BitPtr_PixBox    PixBox;
    
    // very critical loop !!! BitPtr case !!!
    int clms = int(_clms);
    int     counter = 0;
    int     runClr = 0;
    int     n = 0;                    // index of output rlePtr
    
    int ofs32 = 0;
    const PixBox*  ptr32 = getPixBoxPtr_idx(from, &ofs32);
    const char*    ptr08 = reinterpret_cast<const  char*>(ptr32) + ofs32/8;
    int ofs08 = ofs32%8;
    
    for (int c=0; c!=clms; ++c, ++counter) {
        fastForwardSameClr ( runClr, ptr32 , clms , counter, c , ofs32);
        fastForwardSameClr ( runClr, ptr08 , clms , counter, c , ofs08);
        
        int pixClr = from[c];
        
        if (pixClr != runClr) {     // i.e. color changed
            *rlePtr++ = counter;    // store accumulated counter
            ++n;
            counter = 0;            // start new counting
            runClr = pixClr;
        }
    }
    
    *rlePtr++ = counter; // flush last counter
    *rlePtr = 0;         // terminating zero-run     // should be R(0)
    return ++n;
}


///////////////////////////////////////////////////////////////////////////////
/// convert buffer of ANY pixels to BINARY pixels, looking on INPUT pixels
/// as ZEROS and NON-ZEROS
///////////////////////////////////////////////////////////////////////////////
template < class PixConstPtr >
void packFrom(PixConstPtr pixRowConstPtr,BitPtr bits, size_t _clms)
{
    int    clms = int(_clms);
    bool   clr = 0;
    int    run = 0;
    // very CRITICAL loop - could use speeding up
    for (int c=0; c < clms; ++c, ++run)     {
        bool currClr = pixRowConstPtr[c];
        if (currClr != clr) {
            std::fill(bits, bits+run , clr);
            bits += run;
            clr = currClr;
            run = 0;
        }
    }
    if (run)
        std::fill(bits , bits+run , clr);
}


///////////////////////////////////////////////////////////////////////////////
/// pack pointed bits to runs, returning # of clms.
/// 1st run is always BLACK, i.e. color 0, and ONLY 1st run can have 0 length
/// (iff 1st pointed bit is 1).
/// If nRuns NOT given ==> work till zero run (but always after rlePtr[0])
///////////////////////////////////////////////////////////////////////////////
template < class InItor > // R should be any integral type (int,uchar,uint,schar,etc...)
int runs2bits(InItor rlePtr , BitPtr bits, size_t _nRuns = ~0)
{
    int    nRuns = int(_nRuns);
    int    clms = 0;
    bool   color = false;
    for (int i=0; i != nRuns; (color = !color),++i) {
        int run = int(*rlePtr++);
        std::fill(bits,bits+run,color);
        bits += run;
        clms += run;
        if (run==0 && i)
            break;
    }
    return clms;
}


/// ---------- END OF BitRef & BitPtr declarations/definitions --------
/// ---------- specializations of std algorithms for BitPtr -----------
namespace std {
    template <> inline
    BitPtr find(BitPtr frst, BitPtr last, const bool& f)
    {
        typedef BitPtr_PixBox    PixBox;
        enum {boxBits_e = sizeof(PixBox)*8};   // 32
        
        int n = int(last-frst);
        PixBox negator = f ? 0 : ~0; // we use negator to work with ONES(not with ZEROS)
        
        int idx = 0;
        PixBox* ptr = getPixBoxPtr_idx(frst,&idx);
        
        PixBox bit = PixBox(1 << idx);
        
        PixBox curBox = (*ptr ^ negator);   // take 1st box
        if ( curBox & bit) {    // is 1st bit equal f ?
            return frst;        // -yes, very fast return when curr bit is v
        }
        curBox &= ~(bit - 1);   // kill LSB before MY 1st bit
        
        BitPtr p = frst;       // assume RESULT is equal to this
        if ( curBox ) {         // still non-empty ?
            // find 1st v-bit inside 1st PixBox (with killed prev LSB)
            int fst1idx = first1from(curBox);
            if (fst1idx > idx+n)    // is after LAST clm ?
                fst1idx = idx+n;    // -yes : be LAST+1
            
            p += (fst1idx-idx);
            return p;               // OK return computed BitPtr
        }
        
        // curBox is EMPTY and should be skipped
        
        int m = boxBits_e-idx;      // m is # of bits in curBox AFTER starting bit
        if ( n <= m) {              // more than requested ?
            return p += int(n);
        }
        
        n -= m;         // n is NOT 0
        
        ++ptr;          // point to the next PixBox
        // to its starting bit
        
        // skip entire boxes decrementing length 'n'
        for (; n >= boxBits_e && (*ptr^negator)==0 ; ++ptr, n -= boxBits_e) { }
        
        // now either p points to non-empty box (with idx==0) or n<32
        
        int fst1idx = n;     // assume resulting idx to be AFTER_LAST
        
        if (n != 0) {           // requested length still non-zero ?
            // -yes : search in the LAST box
            curBox = (*ptr ^ negator);
            fst1idx = first1from(curBox);
        }
        
        if (fst1idx > n)
            fst1idx = n;
        
        p = makePixelPtr<BitPtr>(ptr) + fst1idx;
        
        return p;
    }
    
    template <> inline
    void fill(BitPtr frst, BitPtr last, const bool& f) {
        typedef BitPtr_PixBox    PixBox;
        
        BitPtr::difference_type n = (last-frst);
        if (n==0)
            return;
        
        //  31 30 29 28 27 26 . . . . . 05 04 03 02 01 00
        // | 0| 0| 0| 0| 0| 0|. . . . .| 0| x| x| x| x| x| <==frontPtr
        //  . . . . . . . . . . . . . . . . . . . . . . .  <==intermediate
        // | 0| 0| 0| 0| 0| 0|. . . . .| 0| 0| 0| 0| 0| 0| <==intermediate
        //  . . . . . . . . . . . . . . . . . . . . . . .  <==intermediate
        // | x| x| x| x| x| 0|. . . . .| 0| 0| 0| 0| 0| 0| <==back_Ptr
        //
        // here  frontBitNo   = 5          ,  back_BitNo = 27
        //       oldFrontMask = 0x0000001F ,oldBack_Mask = 0xF8000000
        int frontBitNo = 0;
        int back_BitNo = 0;
        
        PixBox* frontPtr = getPixBoxPtr_idx(frst  , &frontBitNo);
        PixBox* back_Ptr = getPixBoxPtr_idx(last-1, &back_BitNo);
        
        PixBox  oldFrontMask = (1<<frontBitNo)-1;
        PixBox  oldFrontData = *frontPtr & oldFrontMask;
        
        PixBox  oldBack_Mask = ~(((1<<back_BitNo)<<1)-1);
        PixBox  oldBack_Data = *back_Ptr & oldBack_Mask;
        
        // now fill [frontPtr ... back_Ptr] by given value
        PixBox v = f ? PixBox(~0) : PixBox(0);
        for (PixBox* p = frontPtr; p<=back_Ptr; ++p)
            *p = v;
        // now fix old data in frontPtr and back_Ptr
        if (oldFrontMask)
            *frontPtr = oldFrontData | (*frontPtr & ~oldFrontMask);
        
        if (oldBack_Mask)
            *back_Ptr = oldBack_Data | (*back_Ptr & ~oldBack_Mask);
    }
    
}
/// ---------- END OF specializations of std algorithms ---------------

template <class Pix> struct PixelTypes;

/// WRAPPED pixels for built-in types
template<class T> struct WRPpixelT;

typedef WRPpixelT<unsigned      char >   BWRPpixel;
typedef WRPpixelT<  signed      char >   SWRPpixel;
typedef WRPpixelT<              short>   HWRPpixel;
typedef WRPpixelT<unsigned      short>   WWRPpixel;
typedef WRPpixelT<              int  >   IWRPpixel;
typedef WRPpixelT<unsigned      int  >   UWRPpixel;
typedef WRPpixelT<              long >   LWRPpixel;
typedef WRPpixelT<unsigned      long >   GWRPpixel;
typedef WRPpixelT<         long long >   KWRPpixel;
typedef WRPpixelT<unsigned long long >   QWRPpixel;
typedef WRPpixelT<              float>   FWRPpixel;
typedef WRPpixelT<             double>   DWRPpixel;

/// 3color pixels for built-in types
template<class T> struct            RGBpixelT;
typedef RGBpixelT<unsigned      char >    RGBpixel;
typedef RGBpixelT<  signed      char >   SRGBpixel;
typedef RGBpixelT<              short>   HRGBpixel;
typedef RGBpixelT<unsigned      short>   WRGBpixel;
typedef RGBpixelT<              int  >   IRGBpixel;
typedef RGBpixelT<unsigned      int  >   URGBpixel;
typedef RGBpixelT<              long >   LRGBpixel;
typedef RGBpixelT<unsigned      long >   GRGBpixel;
typedef RGBpixelT<         long long >   KRGBpixel;
typedef RGBpixelT<unsigned long long >   QRGBpixel;
typedef RGBpixelT<              float>   FRGBpixel;
typedef RGBpixelT<             double>   DRGBpixel;

/// 4channel pixel of 4 bytes
struct ARGBpixel;

template<class PixA , class PixB> void minWithPixel(PixA& a , const PixB& b);
template<class PixA , class PixB> void maxWithPixel(PixA& a , const PixB& b);
template<class PixA , class PixB> void sqrt(PixA in, PixB& res);

/// PIXEL SPECIALIZED ONLY FOR BitRef

template<class T> struct PixelTypeX                    { typedef T    Pixel;};
template<>        struct PixelTypeX<BitPtr::reference> { typedef bool Pixel;};

/// WRAPPED TYPE & ITS SPECIALIZATIONS
// !!! PAY ATTENTION:
//     ARGBpixel will loose its 4th-alpha-channel (i.e. will be transformed into appropiate RGBpixelT)
//     in ALL pixel-type transformations: signed/unsigned, widen/narrowed, wrapped, real-valued-pixel
///
template<class T> struct PixelTypeWRP                 { typedef WRPpixelT<T> wrapped_pixel_type;};

template<class T> struct PixelTypeWRP<WRPpixelT<T> >  { typedef WRPpixelT<T> wrapped_pixel_type;};
template<class T> struct PixelTypeWRP<RGBpixelT<T> >  { typedef RGBpixelT<T> wrapped_pixel_type;};

template<> struct PixelTypeWRP< ARGBpixel > { typedef RGBpixelT<unsigned char>  wrapped_pixel_type;};

/// NUMBER-OF-CHANNELS (by default 1) TYPE & ITS SPECIALIZATIONS for RGB (3) & ARGB (4)

template<class T>
struct PixelTypeE  { enum {n_channels_e = 1};};

template<class T> struct PixelTypeE< RGBpixelT<T> > { enum {n_channels_e = 3}; };
template<>        struct PixelTypeE< ARGBpixel >    { enum {n_channels_e = 4};};

/// CHANNEL-VALUE-TYPE & ITS SPECIALIZATIONS

template<class T> struct PixelTypeC  { typedef T    channel_value_type;};

template<class T> struct PixelTypeC<WRPpixelT<T> > { typedef T channel_value_type;};
template<class T> struct PixelTypeC<RGBpixelT<T> > { typedef T channel_value_type;};
template<>        struct PixelTypeC< ARGBpixel >   { typedef unsigned char  channel_value_type;};

/// UNSIGNED TYPE & ITS SPECIALIZATIONS

template<class T> struct PixelTypeU  { typedef T    unsigned_value_type;};

template<> struct PixelTypeU<  signed char > { typedef unsigned      char   unsigned_value_type;};
template<> struct PixelTypeU<         char > { typedef unsigned      char   unsigned_value_type;};
template<> struct PixelTypeU<         short> { typedef unsigned      short  unsigned_value_type;};
template<> struct PixelTypeU<         int  > { typedef unsigned      int    unsigned_value_type;};
template<> struct PixelTypeU<         long > { typedef unsigned      long   unsigned_value_type;};
template<> struct PixelTypeU<    long long > { typedef unsigned long long   unsigned_value_type;};

template<class T> struct PixelTypeU<WRPpixelT<T> >      { typedef WRPpixelT<typename PixelTypeU<T>::unsigned_value_type> unsigned_value_type; };
template<class T> struct PixelTypeU<RGBpixelT<T> >      { typedef RGBpixelT<typename PixelTypeU<T>::unsigned_value_type> unsigned_value_type; };
template<>        struct PixelTypeU< ARGBpixel >        { typedef RGBpixelT<unsigned char>    unsigned_value_type;};

///   SIGNED TYPE & ITS SPECIALIZATIONS

template<class T> struct PixelTypeS  { typedef T      signed_value_type;};
// ---------------------------------------------------------------------------------------------------------------------
template<> struct PixelTypeS<unsigned      char > { typedef signed char   signed_value_type;};
template<> struct PixelTypeS<              char > { typedef signed char   signed_value_type;};
template<> struct PixelTypeS<unsigned      short> { typedef        short  signed_value_type;};
template<> struct PixelTypeS<unsigned      int  > { typedef        int    signed_value_type;};
template<> struct PixelTypeS<unsigned      long > { typedef        long   signed_value_type;};
template<> struct PixelTypeS<unsigned long long > { typedef   long long   signed_value_type;};

template<class T> struct PixelTypeS<WRPpixelT<T> >      { typedef WRPpixelT<typename PixelTypeS<T>::signed_value_type> signed_value_type; };
template<class T> struct PixelTypeS<RGBpixelT<T> >      { typedef RGBpixelT<typename PixelTypeS<T>::signed_value_type> signed_value_type; };
template<>        struct PixelTypeS< ARGBpixel >        { typedef RGBpixelT<signed char >   signed_value_type;};

///   WIDE TYPE & ITS SPECIALIZATIONS

template<class T> struct PixelTypeW  { typedef T      wide_value_type;};
// ---------------------------------------------------------------------------------------------------------------------
template<> struct PixelTypeW<         bool > { typedef unsigned      char    wide_value_type;};
template<> struct PixelTypeW<unsigned char > { typedef unsigned      short   wide_value_type;};
template<> struct PixelTypeW<  signed char > { typedef               short   wide_value_type;};
template<> struct PixelTypeW<         char > { typedef               short   wide_value_type;};
template<> struct PixelTypeW<unsigned short> { typedef unsigned      int     wide_value_type;};
template<> struct PixelTypeW<         short> { typedef               int     wide_value_type;};
template<> struct PixelTypeW<unsigned int  > { typedef unsigned      long    wide_value_type;};
template<> struct PixelTypeW<         int  > { typedef               long    wide_value_type;};
template<> struct PixelTypeW<unsigned long > { typedef unsigned long long    wide_value_type;};
template<> struct PixelTypeW<         long > { typedef          long long    wide_value_type;};

// ---------------------------------------------------------------------------------------------------------------------

template<class T> struct PixelTypeW<WRPpixelT<T> >      { typedef WRPpixelT<typename PixelTypeW<T>::wide_value_type> wide_value_type; };
template<class T> struct PixelTypeW<RGBpixelT<T> >      { typedef RGBpixelT<typename PixelTypeW<T>::wide_value_type> wide_value_type; };
template<>        struct PixelTypeW< ARGBpixel >        { typedef RGBpixelT<unsigned short>   wide_value_type;};

/// NARROW TYPE & ITS SPECIALIZATIONS

template<class T> struct PixelTypeN  { typedef T      narrow_value_type;};

// ---------------------------------------------------------------------------------------------------------------------

template<> struct PixelTypeN<unsigned      short> { typedef unsigned char    narrow_value_type;};
template<> struct PixelTypeN<              short> { typedef   signed char    narrow_value_type;};
template<> struct PixelTypeN<unsigned      int  > { typedef unsigned short   narrow_value_type;};
template<> struct PixelTypeN<              int  > { typedef          short   narrow_value_type;};
template<> struct PixelTypeN<unsigned      long > { typedef unsigned int     narrow_value_type;};
template<> struct PixelTypeN<              long > { typedef          int     narrow_value_type;};
template<> struct PixelTypeN<unsigned long long > { typedef unsigned long    narrow_value_type;};
template<> struct PixelTypeN<         long long > { typedef          long    narrow_value_type;};

// ---------------------------------------------------------------------------------------------------------------------

template<class T> struct PixelTypeN<WRPpixelT<T> >  { typedef WRPpixelT<typename PixelTypeN<T>::narrow_value_type> narrow_value_type; };
template<class T> struct PixelTypeN<RGBpixelT<T> >  { typedef RGBpixelT<typename PixelTypeN<T>::narrow_value_type> narrow_value_type; };
template<>        struct PixelTypeN< ARGBpixel >    { typedef RGBpixelT<unsigned char >   narrow_value_type;};
// ---------------------------------------------------------------------------------------------------------------------

///   REAL TYPE & ITS SPECIALIZATIONS

template<class T> struct PixelTypeR  { typedef T      real_value_type;};
// ---------------------------------------------------------------------------------------------------------------------

template<> struct PixelTypeR<              bool  > { typedef      float   real_value_type;};
template<> struct PixelTypeR<unsigned      char  > { typedef      float   real_value_type;};
template<> struct PixelTypeR<  signed      char  > { typedef      float   real_value_type;};
template<> struct PixelTypeR<              char  > { typedef      float   real_value_type;};
template<> struct PixelTypeR<unsigned      short > { typedef      float   real_value_type;};
template<> struct PixelTypeR<              short > { typedef      float   real_value_type;};
template<> struct PixelTypeR<unsigned      int   > { typedef      float   real_value_type;};
template<> struct PixelTypeR<              int   > { typedef      float   real_value_type;};
template<> struct PixelTypeR<unsigned      long  > { typedef      float   real_value_type;};
template<> struct PixelTypeR<              long  > { typedef      float   real_value_type;};
template<> struct PixelTypeR<unsigned long long  > { typedef      double  real_value_type;};
template<> struct PixelTypeR<         long long  > { typedef      double  real_value_type;};
template<> struct PixelTypeR<              float > { typedef      float   real_value_type;};
template<> struct PixelTypeR<              double> { typedef      double  real_value_type;};
template<> struct PixelTypeR<         long double> { typedef long double  real_value_type;};

// ---------------------------------------------------------------------------------------------------------------------

template<class T> struct PixelTypeR<WRPpixelT<T> >  { typedef WRPpixelT<typename PixelTypeR<T>::real_value_type> real_value_type; };
template<class T> struct PixelTypeR<RGBpixelT<T> >  { typedef RGBpixelT<typename PixelTypeR<T>::real_value_type> real_value_type; };
template<>        struct PixelTypeR< ARGBpixel >    { typedef RGBpixelT<      float >  real_value_type;};

///   DOUBLE-PRECISION TYPE & ITS SPECIALIZATIONS

template<class T> struct PixelTypeD  { typedef T      dblp_value_type;};

template<> struct PixelTypeD<              bool  > { typedef      double  dblp_value_type;};
template<> struct PixelTypeD<unsigned      char  > { typedef      double  dblp_value_type;};
template<> struct PixelTypeD<  signed      char  > { typedef      double  dblp_value_type;};
template<> struct PixelTypeD<              char  > { typedef      double  dblp_value_type;};
template<> struct PixelTypeD<unsigned      short > { typedef      double  dblp_value_type;};
template<> struct PixelTypeD<              short > { typedef      double  dblp_value_type;};
template<> struct PixelTypeD<unsigned      int   > { typedef      double  dblp_value_type;};
template<> struct PixelTypeD<              int   > { typedef      double  dblp_value_type;};
template<> struct PixelTypeD<unsigned      long  > { typedef      double  dblp_value_type;};
template<> struct PixelTypeD<              long  > { typedef      double  dblp_value_type;};
template<> struct PixelTypeD<unsigned long long  > { typedef      double  dblp_value_type;};
template<> struct PixelTypeD<         long long  > { typedef      double  dblp_value_type;};
template<> struct PixelTypeD<              float > { typedef      double  dblp_value_type;};
template<> struct PixelTypeD<              double> { typedef      double  dblp_value_type;};
template<> struct PixelTypeD<         long double> { typedef long double  dblp_value_type;};

template<class T> struct PixelTypeD<WRPpixelT<T> > { typedef WRPpixelT<typename PixelTypeD<T>::dblp_value_type> dblp_value_type; };
template<class T> struct PixelTypeD<RGBpixelT<T> > { typedef RGBpixelT<typename PixelTypeD<T>::dblp_value_type> dblp_value_type; };
template<>        struct PixelTypeD< ARGBpixel >   { typedef RGBpixelT<      double>  dblp_value_type;};

///   min & max values for pixel channels types
template<class T>       // T is any pixel channel type
struct PixelTypeMinMax  {
    typedef T      channel_value_type;
    
    static channel_value_type min()    { return std::numeric_limits<channel_value_type>::min();}
    static channel_value_type max()    { return std::numeric_limits<channel_value_type>::max();}
};

template<> inline      float  PixelTypeMinMax<     float >::min() {return -std::numeric_limits<     float >::max();}
template<> inline      double PixelTypeMinMax<     double>::min() {return -std::numeric_limits<     double>::max();}
template<> inline long double PixelTypeMinMax<long double>::min() {return -std::numeric_limits<long double>::max();}

#ifndef CONVERT_TO_WITHOUT_WARNING
#define CONVERT_TO_WITHOUT_WARNING

template<class PixTarget>
struct convert_to {
    template <class PixSource>
    static PixTarget from(PixSource pix)  { return PixTarget(pix);}
};

template<>
struct convert_to<bool> {
    template <class PixSource>
    static bool from(PixSource pix)  { return int(pix)!=0;}
};
#endif

/// WRAPPED-PIXEL TYPE

// --------------- 1 channel pixel wrapper for arithmetics and common code only

template <class T>      // T should be uchar,char,short, etc...
struct WRPpixelT {
    typedef typename PixelTypeX<T>::Pixel                            Pixel;
    typedef          WRPpixelT<T>                                    value_type;
    typedef typename PixelTypeC<Pixel>::channel_value_type           channel_value_type;
    typedef typename PixelTypeW<channel_value_type>::wide_value_type wide_channel_value_type;
    
    Pixel  v;   // mostly just T (except of BitRef: in this case it is bool)
    
    friend std::ostream& operator<<(std::ostream& os, const value_type& x) {
        return os << x.v ;
    }
    
    friend std::istream& operator>> (std::istream& s,  value_type& x) {
        std::ios::iostate state = std::ios::goodbit;
        const std::istream::sentry ok(s);
        if (ok) {
            try { //extract elements and convert accumulate flags in state store a successful conversion
                s >> x.v ;
            } catch (...) {
                try {
                    s.setstate(std::ios::badbit);
                } catch (...) {}
                if ((s.exceptions() & std::ios::badbit) != 0) throw;
            }
        }
        s.setstate(state);
        return s;
    }
    
    //  ~WRPpixelT()               { }      // d-tor generated by compiler
    
    WRPpixelT()    /* c-tor */         { }
    
    WRPpixelT(              bool   v) /* c-tor */  : v(T(v))  { }
    WRPpixelT(              char   v) /* c-tor */  : v(T(v))  { }
    WRPpixelT(unsigned      char   v) /* c-tor */  : v(T(v))  { }
    WRPpixelT(  signed      char   v) /* c-tor */  : v(T(v))  { }
    WRPpixelT(unsigned      short  v) /* c-tor */  : v(T(v))  { }
    WRPpixelT(              short  v) /* c-tor */  : v(T(v))  { }
    WRPpixelT(unsigned      int    v) /* c-tor */  : v(T(v))  { }
    WRPpixelT(              int    v) /* c-tor */  : v(T(v))  { }
    WRPpixelT(unsigned      long   v) /* c-tor */  : v(T(v))  { }
    WRPpixelT(              long   v) /* c-tor */  : v(T(v))  { }
    WRPpixelT(unsigned long long   v) /* c-tor */  : v(T(v))  { }
    WRPpixelT(         long long   v) /* c-tor */  : v(T(v))  { }
    WRPpixelT(              float  v) /* c-tor */  : v(T(v))  { }
    WRPpixelT(              double v) /* c-tor */  : v(T(v))  { }
    WRPpixelT(         long double v) /* c-tor */  : v(T(v))  { }
    
    //    WRPpixelT(T v) /* c-tor */  : v(T(v))  { }
    
    WRPpixelT(const ARGBpixel& pix);  /* c-tor */
    
    //  WRPpixelT(const WRPpixelT&);              // copy c-tor generated by compiler
    //  WRPpixelT& operator=(const WRPpixelT&);   // assignment generated by compiler
    
    T* data()                        { return &v;}
    const T* data() const                  { return &v;}
    
    template<class S>
    WRPpixelT<T>(const WRPpixelT<S>& pix)
    : v(T(pix.v))
    {
    }
    
    // additive arithmetic
    WRPpixelT<T> operator-() const;//    { return WRPpixelT<T>(-v); }
    
    template<class S>
    WRPpixelT<T>& operator+=(const WRPpixelT<S>& pix) { v += T(pix.v); return *this; }
    
    template<class S>
    WRPpixelT<T>& operator-=(const WRPpixelT<S>& pix) { v -= T(pix.v); return *this; }
    
    // multiplicative arithmetic
    
    template<class S>
    WRPpixelT<T>& operator*=(const WRPpixelT<S>& pix) { v *= T(pix.v); return *this; }
    
    template<class S>
    WRPpixelT<T>& operator/=(const WRPpixelT<S>& pix) { v /= T(pix.v); return *this; }
    
    
    template<class S>
    WRPpixelT<T>& minWith(const WRPpixelT<S>& pix) {
        v = (std::min)( v , T(pix.v));
        return *this;
    }
    
    template<class S>
    WRPpixelT<T>& maxWith(const WRPpixelT<S>& pix) {
        v = (std::max)( v , T(pix.v));
        return *this;
    }
    
    
    // assignment
    template<class S>
    WRPpixelT<T>& operator=(const WRPpixelT<S>& pix) { v = T(pix.v); return *this;}
    
    wide_channel_value_type     sum() const  { return wide_channel_value_type(v);}
    
    wide_channel_value_type   norm2() const  { return sqr();}
    channel_value_type          max() const  { return v;}
    channel_value_type          min() const  { return v;}
    channel_value_type          abs() const;//  { return v<0 ? -v : v;}
    wide_channel_value_type     sqr() const;//  { return wide_channel_value_type(v)*wide_channel_value_type(v);}
    channel_value_type         sqrt() const;//  { return channel_value_type(::sqrt(float(v)));}
    //channel_value_type    log() const;//  { return wide_channel_value_type(v)*wide_channel_value_type(v);}
    //wide_channel_value_type exp() const;//  { return wide_channel_value_type(v)*wide_channel_value_type(v);}
    
    operator     channel_value_type() const  { return channel_value_type(sum());}
    
    operator ARGBpixel() const;
    
    // assignment operators from built-in-types
    
    void operator=(              bool   u)       { v = T(u) ;}
    void operator=(              char   u)       { v = T(u) ;}
    void operator=(unsigned      char   u)       { v = T(u) ;}
    void operator=(  signed      char   u)       { v = T(u) ;}
    void operator=(unsigned      short  u)       { v = T(u) ;}
    void operator=(              short  u)       { v = T(u) ;}
    void operator=(unsigned      int    u)       { v = T(u) ;}
    void operator=(              int    u)       { v = T(u) ;}
    void operator=(unsigned      long   u)       { v = T(u) ;}
    void operator=(              long   u)       { v = T(u) ;}
    void operator=(unsigned long long   u)       { v = T(u) ;}
    void operator=(         long long   u)       { v = T(u) ;}
    void operator=(              float  u)       { v = T(u) ;}
    void operator=(              double u)       { v = T(u) ;}
    void operator=(         long double u)       { v = T(u) ;}
    
    
    const T  operator[](size_t i) const { assert(i==0); return (&v)[i];}
    T& operator[](size_t i)       { assert(i==0); return (&v)[i];}
    
    template<class S>
    bool operator==(const WRPpixelT<S>& pix) const { return v==T(pix.v);}
    
    template<class S>
    bool operator< (const WRPpixelT<S>& pix) const { return v< T(pix.v);}
};

template<class T> inline
WRPpixelT<T> WRPpixelT<T>::operator-() const    { return WRPpixelT<T>(-v); }

template<class T> inline
typename WRPpixelT<T>::channel_value_type        WRPpixelT<T>::abs () const  { return v<0 ? -v : v;}

template<class T> inline
typename WRPpixelT<T>::wide_channel_value_type   WRPpixelT<T>::sqr () const  { wide_channel_value_type w = convert_to<wide_channel_value_type>::from(Pixel(v)); return w*w;}

template<class T> inline
typename WRPpixelT<T>::channel_value_type        WRPpixelT<T>::sqrt() const  { return channel_value_type(::sqrt(float(v)));}

template<> inline void WRPpixelT<bool>::operator=(              bool   u)       { v = u    ;}
template<> inline void WRPpixelT<bool>::operator=(              char   u)       { v = u!=0 ;}
template<> inline void WRPpixelT<bool>::operator=(unsigned      char   u)       { v = u!=0 ;}
template<> inline void WRPpixelT<bool>::operator=(  signed      char   u)       { v = u!=0 ;}
template<> inline void WRPpixelT<bool>::operator=(unsigned      short  u)       { v = u!=0 ;}
template<> inline void WRPpixelT<bool>::operator=(              short  u)       { v = u!=0 ;}
template<> inline void WRPpixelT<bool>::operator=(unsigned      int    u)       { v = u!=0 ;}
template<> inline void WRPpixelT<bool>::operator=(              int    u)       { v = u!=0 ;}
template<> inline void WRPpixelT<bool>::operator=(unsigned      long   u)       { v = u!=0 ;}
template<> inline void WRPpixelT<bool>::operator=(              long   u)       { v = u!=0 ;}
template<> inline void WRPpixelT<bool>::operator=(unsigned long long   u)       { v = u!=0 ;}
template<> inline void WRPpixelT<bool>::operator=(         long long   u)       { v = u!=0 ;}
template<> inline void WRPpixelT<bool>::operator=(              float  u)       { v = u!=0 ;}
template<> inline void WRPpixelT<bool>::operator=(              double u)       { v = u!=0 ;}
template<> inline void WRPpixelT<bool>::operator=(         long double u)       { v = u!=0 ;}

template<> inline
WRPpixelT<bool>::operator channel_value_type() const  { return v!=0;}

/// --------------- RGB pixel types ---------------------------
//////////////////////////////////////////////////////////////////////////
// *-----------------------*-----------------------*-----------------------*
// |            R          |            G          |            B          |
// *-----------------------*-----------------------*-----------------------*
// |                       |                       |                       |
// |23 22 21 20 19 18 17 16|15 14 13 12 11 10  9  8| 7  6  5  4  3  2  1  0|
//////////////////////////////////////////////////////////////////////////
///

template <class T>
struct RGBpixelT {
    typedef     RGBpixelT<T>   value_type;
    
    typedef typename PixelTypeC<value_type>        ::channel_value_type  channel_value_type;
    typedef typename PixelTypeW<channel_value_type>::wide_value_type     wide_channel_value_type;
    
    friend std::ostream& operator<<(std::ostream& os, const value_type& v) {
        int w = int(os.width());
        return os << v.r << " " << std::setw(w) << v.g << " " << std::setw(w) << v.b ;
    }
    
    friend std::istream& operator>> (std::istream& s,  value_type& v) {
        std::ios::iostate state = std::ios::goodbit;
        const std::istream::sentry ok(s);
        if (ok) {
            try { //extract elements and convert accumulate flags in state store a successful conversion
                s >> v.r; if(s.rdstate() & (std::ios::badbit | std::ios::failbit | std::ios::eofbit))   throw std::underflow_error("RGBpixel: red   expected");
                s >> v.g; if(s.rdstate() & (std::ios::badbit | std::ios::failbit | std::ios::eofbit))   throw std::underflow_error("RGBpixel: green expected");
                s >> v.b; if(s.rdstate() & (std::ios::badbit | std::ios::failbit                   ))   throw std::underflow_error("RGBpixel: blue  expected");
            } catch (...) {
                try {
                    s.setstate(std::ios::badbit);
                } catch (...) {}
                if ((s.exceptions() & std::ios::badbit) != 0) throw;
            }
        }
        s.setstate(state);
        return s;
    }
    
    
    T b;
    T g;
    T r;
    
    static int   red  ()     { return 2;}
    static int   green()     { return 1;}
    static int   blue ()     { return 0;}
    
    //  ~RGBpixelT()               { }      // d-tor generated by compiler
    
    RGBpixelT()    /* c-tor */         { }
    
    //    RGBpixelT(T v) /* c-tor */         { r=g=b=T(v); }
    RGBpixelT(               bool   v) /* c-tor */         { r=g=b=T(v); }
    RGBpixelT(               char   v) /* c-tor */         { r=g=b=T(v); }
    RGBpixelT( unsigned      char   v) /* c-tor */         { r=g=b=T(v); }
    RGBpixelT(   signed      char   v) /* c-tor */         { r=g=b=T(v); }
    RGBpixelT( unsigned      short  v) /* c-tor */         { r=g=b=T(v); }
    RGBpixelT(               short  v) /* c-tor */         { r=g=b=T(v); }
    RGBpixelT( unsigned      int    v) /* c-tor */         { r=g=b=T(v); }
    RGBpixelT(               int    v) /* c-tor */         { r=g=b=T(v); }
    RGBpixelT( unsigned      long   v) /* c-tor */         { r=g=b=T(v); }
    RGBpixelT(               long   v) /* c-tor */         { r=g=b=T(v); }
    RGBpixelT( unsigned long long   v) /* c-tor */         { r=g=b=T(v); }
    RGBpixelT(          long long   v) /* c-tor */         { r=g=b=T(v); }
    RGBpixelT(               float  v) /* c-tor */         { r=g=b=T(v); }
    RGBpixelT(               double v) /* c-tor */         { r=g=b=T(v); }
    RGBpixelT(          long double v) /* c-tor */         { r=g=b=T(v); }
    
    template<class S>
    RGBpixelT(const RGBpixelT<S>& pix)       /* c-tor*/
    : r(T(pix.r)), g(T(pix.g)), b(T(pix.b))
    { }
    
    template<class S>
    RGBpixelT(S r , S g , S b)  /* c-tor */
    : r(T(r)), g(T(g)), b(T(b))
    { }
    
    RGBpixelT(const ARGBpixel& pix);  /* c-tor */
    
    //  RGBpixelT(const RGBpixelT&);              // copy c-tor generated by compiler
    //  RGBpixelT& operator=(const RGBpixelT&);   // assignment generated by compiler
    
    T*       data()                        { return &b;}
    const T* data() const                  { return &b;}
    
    
    // two's complement
    RGBpixelT<T> operator~() const {
        return RGBpixelT<T>(~r,~g,~b);
    }
    
    
    // additive arithmetic
    RGBpixelT<T> operator-() const {
        return RGBpixelT<T>(-r,-g,-b);
    }
    
    template<class S>
    RGBpixelT<T>& operator+=(const RGBpixelT<S>& pix) {
        r += T(pix.r);
        g += T(pix.g);
        b += T(pix.b);
        return *this;
    }
    
    template<class S>
    RGBpixelT<T>& operator-=(const RGBpixelT<S>& pix) {
        r -= T(pix.r);
        g -= T(pix.g);
        b -= T(pix.b);
        return *this;
    }
    
    // multiplicative arithmetic
    
    template<class S>
    RGBpixelT<T>& operator*=(const RGBpixelT<S>& pix) {
        r *= T(pix.r);
        g *= T(pix.g);
        b *= T(pix.b);
        return *this;
    }
    
    
    template<class S>
    RGBpixelT<T>& operator/=(const RGBpixelT<S>& pix) {
        r /= T(pix.r);
        g /= T(pix.g);
        b /= T(pix.b);
        return *this;
    }
    
    
    template<class S>
    RGBpixelT<T>& minWith(const RGBpixelT<S>& pix) {
        r = (std::min)( r , T(pix.r));
        g = (std::min)( g , T(pix.g));
        b = (std::min)( b , T(pix.b));
        return *this;
    }
    
    template<class S>
    RGBpixelT<T>& maxWith(const RGBpixelT<S>& pix) {
        r = (std::max)( r , T(pix.r));
        g = (std::max)( g , T(pix.g));
        b = (std::max)( b , T(pix.b));
        return *this;
    }
    
    RGBpixelT<T> abs() const {
        RGBpixelT<T> absPix;
        absPix.r = r<T(0) ? -r : r;
        absPix.g = g<T(0) ? -g : g;
        absPix.b = b<T(0) ? -b : b;
        return absPix;
    }
    
    RGBpixelT<T> sqrt() const {
        RGBpixelT<T> res;
        res.r = channel_value_type(::sqrt(float(r)));
        res.g = channel_value_type(::sqrt(float(g)));
        res.b = channel_value_type(::sqrt(float(b)));
        return res;
    }
    
    
    RGBpixelT<wide_channel_value_type> sqr() const {
        RGBpixelT<wide_channel_value_type> res;
        res.r =  wide_channel_value_type(r)*wide_channel_value_type(r) ;
        res.g =  wide_channel_value_type(g)*wide_channel_value_type(g) ;
        res.b =  wide_channel_value_type(b)*wide_channel_value_type(b) ;
        return res;
    }
    
    wide_channel_value_type   sum() const { return wide_channel_value_type(r)+wide_channel_value_type(g)+wide_channel_value_type(b);}
    
    wide_channel_value_type norm2() const
    {
        return wide_channel_value_type(r)*wide_channel_value_type(r) +
        wide_channel_value_type(g)*wide_channel_value_type(g) +
        wide_channel_value_type(b)*wide_channel_value_type(b);
    }
    
    channel_value_type    min() const  { return (std::min)(r, (std::min)(g,b));}
    
    channel_value_type    max() const  { return (std::max)(r, (std::max)(g,b));}
    
    // assignment
    template<class S>
    RGBpixelT<T>& operator=(const RGBpixelT<S>& pix) {
        r = T(pix.r);
        g = T(pix.g);
        b = T(pix.b);
        return *this;
    }
    
    operator channel_value_type() const   { return sum()/wide_channel_value_type(3);}
    
    operator ARGBpixel() const;
    
    
    // assignment operators from built-in-types
    
    void operator=(              bool   u)       { r=g=b=T(u);}
    void operator=(              char   u)       { r=g=b=T(u);}
    void operator=(unsigned      char   u)       { r=g=b=T(u);}
    void operator=(  signed      char   u)       { r=g=b=T(u);}
    void operator=(unsigned      short  u)       { r=g=b=T(u);}
    void operator=(              short  u)       { r=g=b=T(u);}
    void operator=(unsigned      int    u)       { r=g=b=T(u);}
    void operator=(              int    u)       { r=g=b=T(u);}
    void operator=(unsigned      long   u)       { r=g=b=T(u);}
    void operator=(              long   u)       { r=g=b=T(u);}
    void operator=(unsigned long long   u)       { r=g=b=T(u);}
    void operator=(         long long   u)       { r=g=b=T(u);}
    void operator=(              float  u)       { r=g=b=T(u);}
    void operator=(              double u)       { r=g=b=T(u);}
    void operator=(         long double u)       { r=g=b=T(u);}
    
    
    T  operator[](size_t i) const { return at(i);}
    T& operator[](size_t i)       { return at(i);}
    
    // alternative notation for subscripting
    T  at(size_t i) const         { return (&b)[i];}
    T& at(size_t i)               { return (&b)[i];}
    
    template<class S>
    bool operator==(const RGBpixelT<S>& pix) const {
        return r==T(pix.r) && g==T(pix.g) && b==T(pix.b);
    }
    
    template<class S> //! !!! limited usage : in very special cases e.g. checking negativeness
    bool operator< (const RGBpixelT<S>& pix) const {
        return r<T(pix.r) && g<T(pix.g) && b<T(pix.b);
    }
    
    //! usage of the following will issue a compiler error
    template<class S> bool operator<=(const RGBpixelT<S>& pix) const {   static_assert(std::is_same<S, void>::value, "RGBpixelT<S>::operator<= : illigal operation! use only operators '<' or '>'");  return false;   }
    template<class S> bool operator>=(const RGBpixelT<S>& pix) const {   static_assert(std::is_same<S, void>::value, "RGBpixelT<S>::operator<= : illigal operation! use only operators '<' or '>'");  return false;   }
};

using namespace std::rel_ops;   // to get != for equalityComparable and > <= >= for lessThanComparable (Note: usage of <=, >= has been explicitely disabled for RGPpixelT)

/// --------------- ARGB pixel type--- -------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////
// *-----------------------*-----------------------*-----------------------*-----------------------*
// |            A          |            R          |            G          |            B          |
// *-----------------------*-----------------------*-----------------------*-----------------------*
// |                       |                       |                       |                       |
// |31 30 29 28 27 26 25 24|23 22 21 20 19 18 17 16|15 14 13 12 11 10  9  8| 7  6  5  4  3  2  1  0|
////////////////////////////////////////////////////////////////////////////////////////////////////
/// ------------------------------------------------------------------------------------------------
struct ARGBpixel {
    typedef unsigned char   uchar;
    typedef ARGBpixel       value_type;
    
    uchar b;
    uchar g;
    uchar r;
    uchar a;
    
    //  ~ARGBpixel()               { }      // d-tor generated by compiler
    ARGBpixel()                { }
    
    //    ARGBpixel(int v)           { r=g=b=0; a=uchar(v); }
    ARGBpixel(         bool   v)           { r=g=b=0; a=uchar(v); }
    ARGBpixel(         char   v)           { r=g=b=0; a=uchar(v); }
    ARGBpixel(unsigned char   v)           { r=g=b=0; a=uchar(v); }
    ARGBpixel(  signed char   v)           { r=g=b=0; a=uchar(v); }
    ARGBpixel(unsigned short  v)           { r=g=b=0; a=uchar(v); }
    ARGBpixel(         short  v)           { r=g=b=0; a=uchar(v); }
    ARGBpixel(unsigned int    v)           { r=g=b=0; a=uchar(v); }
    ARGBpixel(         int    v)           { r=g=b=0; a=uchar(v); }
    ARGBpixel(unsigned long   v)           { r=g=b=0; a=uchar(v); }
    ARGBpixel(         long   v)           { r=g=b=0; a=uchar(v); }
    ARGBpixel(         float  v)           { r=g=b=0; a=uchar(v); }
    ARGBpixel(         double v)           { r=g=b=0; a=uchar(v); }
    ARGBpixel(    long double v)           { r=g=b=0; a=uchar(v); }
    
    static int   red  ()     { return 2;}
    static int   green()     { return 1;}
    static int   blue ()     { return 0;}
    static int   alpha()     { return 3;}
    
    ARGBpixel(int _r,int _g, int _b)
    : r(uchar(_r)), g(uchar(_g)), b(uchar(_b)), a(0)
    { }
    
    template <class T>
    ARGBpixel(const RGBpixelT<T>& pix)
    : r(uchar(pix.r)), g(uchar(pix.g)), b(uchar(pix.b)), a(0)
    { }
    
    //  ARGBpixel(const ARGBpixel&);              // copy c-tor generated by compiler
    //  ARGBpixel& operator=(const ARGBpixel&);   // assignment generated by compiler
    
    uchar*       data()        { return &b;}
    const uchar* data() const  { return &b;}
    
    operator int() const       { return a;}
    
    //    operator RGBpixel() const  { RGBpixel rgb; rgb.r=r; rgb.g=g; rgb.b=b; return rgb;}
    
    void operator=(int v)      { r=g=b=0; a=uchar(v);}
    
    uchar  operator[](size_t i) const { return at(i);}
    uchar& operator[](size_t i)       { return at(i);}
    
    // alternative notation for subscripting
    uchar  at(size_t i) const         { return (&b)[i];}
    uchar& at(size_t i)               { return (&b)[i];}
    
    bool operator==(const ARGBpixel& pix) const {
        return r==pix.r && g==pix.g && b==pix.b && a==pix.a;
    }
};

// inline functions of RGBpixelT

template<class T> inline
RGBpixelT<T>::operator ARGBpixel() const
{
    typedef unsigned char  uchar;
    ARGBpixel argb;
    argb.r = uchar(r);
    argb.g = uchar(g);
    argb.b = uchar(b);
    argb.a = uchar(0);
    
    return argb;
}

template<class T> inline
RGBpixelT<T>::RGBpixelT(const ARGBpixel& pix)   /* c-tor */
: r ( T(pix.r) )
, g ( T(pix.g) )
, b ( T(pix.b) )
{
}

// inline functions of WRPpixelT

template<class T> inline
WRPpixelT<T>::operator ARGBpixel() const
{
    typedef unsigned char  uchar;
    ARGBpixel argb;
    argb.r = uchar(0);
    argb.g = uchar(0);
    argb.b = uchar(0);
    argb.a = uchar(v);
    
    return argb;
}

template<class T> inline
WRPpixelT<T>::WRPpixelT(const ARGBpixel& pix)   /* c-tor */
: v ( T(pix.a) )
{
}

///
// TEMPLATE FOR ALL DEPENDANT TYPES OF PIXELS
///

template <class Pix> struct PixelTypes
{
    typedef typename PixelTypeX<Pix>::Pixel                                Pixel;
    
    typedef Pixel                                                          value_type;
    typedef typename PixelTypeWRP<Pixel>::wrapped_pixel_type       wrapped_pixel_type;
    
    typedef typename PixelTypeC<Pixel>::channel_value_type         channel_value_type;
    
    typedef typename PixelTypeU<Pixel>::unsigned_value_type       unsigned_value_type;
    typedef typename PixelTypeS<Pixel>::  signed_value_type         signed_value_type;
    typedef typename PixelTypeW<Pixel>::    wide_value_type           wide_value_type;
    typedef typename PixelTypeN<Pixel>::  narrow_value_type         narrow_value_type;
    typedef typename PixelTypeR<Pixel>::    real_value_type           real_value_type;
    typedef typename PixelTypeD<Pixel>::    dblp_value_type           dblp_value_type;
    
    enum { n_channels_e = PixelTypeE<Pixel>::n_channels_e };
    
    typedef typename PixelTypesHelp<Pixel>::ConstValue_typePtr   ConstValue_typePtr;
    typedef typename PixelTypesHelp<Pixel>::     Value_typePtr        Value_typePtr;
    
    typedef typename PixelTypesHelp<Pixel>::ConstPixelPtr   ConstPixelPtr;
    typedef typename PixelTypesHelp<Pixel>::     PixelPtr        PixelPtr;
    typedef typename PixelTypesHelp<Pixel>::ConstPixelRef   ConstPixelRef;
    typedef typename PixelTypesHelp<Pixel>::     PixelRef        PixelRef;
    
    typedef typename PixelTypesHelp<Pixel>::       PixBox        PixBox;
    //--// PixBox normally contains 1 pixel ( in bool   case - 32 pixels
    //--//                                  , in nibble case -  2 pixels)
    
    enum { pixelsInBox_e  = PixelTypesHelp<Pixel>::pixelsInBox_e};
    
    enum { pixel_bits_e   = PixelTypesHelp<Pixel>::pixel_bits_e};
    enum { channel_bits_e = pixel_bits_e/n_channels_e};
    
    static int nChannels  ()        { return n_channels_e  ; }
    static int pixelsInBox()        { return pixelsInBox_e ; }
    static int pixelBits  ()        { return pixel_bits_e  ; }
    static int channelBits()        { return channel_bits_e; }
    
    static Pix max()                { return Pix (wrapped_pixel_type(PixelTypeMinMax<channel_value_type>::max())); }
    static Pix min()                { return Pix (wrapped_pixel_type(PixelTypeMinMax<channel_value_type>::min())); }
};

/// -------- restore Pixel from PixelPtr or PixelRef ---------------
// default:  typedef typename std::remove_pointer<AnyPixelPtr>::type Pixel;     // (from header <type_traits>)
//
// for BitPtr we define the following specialization
// example of usage:
// typedef typename std::remove_pointer<AnyPixelPtr>::type AnyPixel; // fetch appropiate pixel type
//
// default:  typedef typename std::remove_reference<AnyPixelRef>::type Pixel;     // (from header <type_traits>)
//
// for BitRef we define the following specialization
// example of usage:
// typedef typename std::remove_reference<AnyPixelRef>::type AnyPixel; // fetch appropiate pixel type

namespace std {
    
    template<>
    struct remove_pointer<BitPtr>
    {	// remove pointer
        typedef BitPtr::reference type;
    };
    
    template<>
    struct remove_reference<BitPtr::reference>
    {	// remove reference
        typedef bool type;
    };
}

/// declare & define y-iterators: ConstYiterator<AnyPixelPtr>, Yiterator<AnyPixelPtr>

template <class AnyPixelPtr> class Yiterator;
template <class AnyPixelPtr> class ConstYiterator;



template<class AnyPixelPtr>
struct ConstYiteratorBase : public std::iterator < std::random_access_iterator_tag
, typename std::remove_pointer<typename std::remove_const<AnyPixelPtr>::type>::type
>
{
private:
    
    typedef std::iterator < std::random_access_iterator_tag
    , typename std::remove_pointer<typename std::remove_const<AnyPixelPtr>::type>::type
    >      Base;
    
public:
    
    typedef typename std::remove_const<AnyPixelPtr>::type             AnyPixelNonConstPtr;
    typedef typename std::remove_pointer<AnyPixelNonConstPtr>::type   value_type;
    typedef typename std::remove_pointer<AnyPixelNonConstPtr>::type   Pixel;
    typedef typename Base::reference                                  reference;
    typedef typename Base::pointer                                    pointer;
    typedef typename Base::difference_type                            difference_type;
    
    static AnyPixelNonConstPtr convert2nonConst(AnyPixelPtr p)  {   return const_cast<AnyPixelNonConstPtr>(p);}
};


template<>
struct ConstYiteratorBase<BitPtr> : public std::iterator < std::random_access_iterator_tag
, typename std::remove_pointer<BitPtr>::type
>
{
    typedef BitPtr    AnyPixelNonConstPtr;
    typedef bool      value_type;
    typedef bool      Pixel;
    
    typedef BitPtr::reference  reference;
    typedef BitPtr             pointer;
    typedef int                difference_type;
    
    static AnyPixelNonConstPtr convert2nonConst(BitPtr p)  {   return p;}
};


template<class AnyPixelPtr>
class ConstYiterator : public ConstYiteratorBase <AnyPixelPtr>
{
    typedef ConstYiteratorBase<AnyPixelPtr>           Base;
    typedef ConstYiterator<AnyPixelPtr>               Self;
public:
    typedef typename Base::AnyPixelNonConstPtr   AnyPixelNonConstPtr;
    typedef typename Base::value_type            value_type;
    typedef typename Base::Pixel                 Pixel;
    typedef typename Base::reference             reference;
    typedef typename Base::pointer               pointer;
    typedef typename Base::difference_type       difference_type;
    
    ConstYiterator() : p(0)     { }
    
    ConstYiterator(AnyPixelPtr _p , int _clmJump)
    : p(Base::convert2nonConst(_p)), clmJump(_clmJump)        { }
    
    //      ConstYiterator (const ConstYiterator& v) : p(v.p)    { }     // generated
    reference       operator* ()          const    { return *p; }
    pointer         operator->()          const    { return  p; }
    
    reference operator  [] (int i)  const    { return *((*this)+i) ;}
    
    ConstYiterator& operator++()                   { _increment(1);  return *this; }
    ConstYiterator& operator--()                   { _decrement(1);  return *this; }
    
    ConstYiterator  operator++(int)                { ConstYiterator t = *this; ++*this; return t; }
    ConstYiterator  operator--(int)                { ConstYiterator t = *this; --*this; return t; }
    
    ConstYiterator& operator+=(int n)              { _increment(n); return *this; }
    ConstYiterator& operator-=(int n)              { _decrement(n); return *this; }
    
    ConstYiterator  operator+ (int n)      const   { ConstYiterator t(*this); t+=n; return t;}
    ConstYiterator  operator- (int n)      const   { ConstYiterator t(*this); t-=n; return t;}
    
    difference_type operator- (Self other) const   { return (p - other.p)/clmJump; }
    
    bool operator==(const ConstYiterator& x) const { return p == x.p;      }
    bool operator!=(const ConstYiterator& x) const { return !(*this == x); }
    
    bool operator< (const ConstYiterator& x) const { return p < x.p;       }
    
    void _decrement(int n)                              { p -= clmJump*n; }
    void _increment(int n)                              { p += clmJump*n; }
    
protected:
    AnyPixelNonConstPtr p;
    int                 clmJump;
    
};

template<class AnyPixelPtr>
class Yiterator : public ConstYiterator<AnyPixelPtr> {
    typedef ConstYiterator<AnyPixelPtr>   base;
    typedef Yiterator<AnyPixelPtr>        self;
public:
    typedef typename base::value_type      value_type;
    typedef typename base::Pixel           Pixel;
    typedef typename base::reference       reference;
    typedef typename base::pointer         pointer;
    typedef typename base::difference_type difference_type;
    
    
    Yiterator()                                 { }
    Yiterator(AnyPixelPtr _p, int _clmJump) : base(_p, _clmJump)   { }
    reference        operator* () const              { return *base::p; }
    pointer          operator->() const              { return  base::p; }
    
    reference        operator  [] (int i)  const    { return *((*this)+i) ;}
    reference        operator  [] (int i)           { return *((*this)+i) ;}
    
    Yiterator&       operator++()                   { base::_increment(1);  return *this; }
    Yiterator&       operator--()                   { base::_decrement(1);  return *this; }
    Yiterator        operator++(int)                { Yiterator t(*this); ++*this; return t; }
    Yiterator        operator--(int)                { Yiterator t(*this); --*this; return t; }
    
    Yiterator&       operator+=(int n)              { base::_increment(n); return *this;}
    Yiterator&       operator-=(int n)              { base::_decrement(n); return *this;}
    Yiterator        operator+ (int n)      const   { Yiterator t(*this); t+=n; return t;}
    Yiterator        operator- (int n)      const   { Yiterator t(*this); t-=n; return t;}
    
    difference_type  operator- (self other) const   { return (base::p - other.p)/base::clmJump; }
    
    //    bool operator==(const Yiterator& x) const   { return base::p == x.base::p; }
    //    bool operator!=(const Yiterator& x) const   { return (!(*this == x)); }
};

template <class AnyDB> inline
ConstYiterator<typename AnyDB::ConstPixelPtr> make_ConstYiterator(const AnyDB& roi, int x) {
    typedef typename AnyDB::ConstPixelPtr  ConstPixelPtr;
    ConstYiterator<ConstPixelPtr> p (roi[0]+x , int(roi.getImg()->getWidth()));
    return p;
}


template <class AnyDB> inline
Yiterator<typename AnyDB::PixelPtr> make_Yiterator(AnyDB& roi, int x) {
    typedef typename AnyDB::PixelPtr       PixelPtr;
    Yiterator<PixelPtr> p (roi[0]+x , int(roi.getImg()->getWidth()));
    return p;
}

/// declare & define adapter functions make_ConstXiterator, make_Xiterator
template <class AnyDB> inline
typename AnyDB::ConstPixelPtr make_ConstXiterator( const AnyDB& roi, int y) {
    typedef typename AnyDB::ConstPixelPtr  ConstPixelPtr;
    ConstPixelPtr p  = roi[y];
    return p;
}

template <class AnyDB> inline
typename AnyDB::PixelPtr make_Xiterator( AnyDB& roi, int y) {
    typedef typename AnyDB::PixelPtr  PixelPtr;
    PixelPtr p  = roi[y];
    return p;
}

/// -------------------------------------------------------------------------------------------------

/// declare & define abs(a,b) global functions

template<class PixA, class PixB>
inline void _absPix(PixA in, PixB& res)
{
    typedef typename PixelTypes<PixA>::wrapped_pixel_type wrappedA_t;
    typedef typename PixelTypes<PixB>::wrapped_pixel_type wrappedB_t;
    res = wrappedA_t(in).abs();
}

/// declare & define sqrt(a,b) global functions
template<class PixA, class PixB> inline void _sqrtPix(PixA in, PixB& res)
{
    typedef typename PixelTypes<PixA>::wrapped_pixel_type wrappedA_t;
    typedef typename PixelTypes<PixB>::wrapped_pixel_type wrappedB_t;
    res = wrappedA_t(in).sqrt();
}

/// declare & define MIN-MAX(a,b) global functions

template<class PixA , class PixB> inline
void maxWithPixel(PixA& a , const PixB& b)
{
    typedef typename PixelTypes<PixA>::wrapped_pixel_type wrappedA_t;
    typedef typename PixelTypes<PixB>::wrapped_pixel_type wrappedB_t;
    wrappedA_t res = a;
    res.maxWith(wrappedB_t(b));
    a = res;
}

template<class PixA , class PixB> inline
void minWithPixel(PixA& a , const PixB& b)
{
    typedef typename PixelTypes<PixA>::wrapped_pixel_type wrappedA_t;
    typedef typename PixelTypes<PixB>::wrapped_pixel_type wrappedB_t;
    wrappedA_t res = a;
    res.minWith(wrappedB_t(b));
    a = res;
}


//namespace std {
//    inline NibblePixel<UnsignedPixel> std::numeric_limits<NibblePixel<UnsignedPixel> >::min()  { return  0;}
//    inline NibblePixel<UnsignedPixel> std::numeric_limits<NibblePixel<UnsignedPixel> >::max()  { return 15;}
//    inline NibblePixel<  SignedPixel> std::numeric_limits<NibblePixel<  SignedPixel> >::min()  { return -8;}
//    inline NibblePixel<  SignedPixel> std::numeric_limits<NibblePixel<  SignedPixel> >::max()  { return  7;}
//}

/// -------------------------------------------------------------------------------
// template functions applyLUT , applyLUT_RGB
/// -------------------------------------------------------------------------------

template <class Pix, class BracketableIntArray>
inline Pix applyLUT    (Pix pix, BracketableIntArray lut)
{
    typedef typename PixelTypes<Pix>::wrapped_pixel_type  wrapped_pixel_type;
    wrapped_pixel_type wpix = pix;
    enum { n_channels_e = PixelTypes<Pix>::n_channels_e };
    int nchannels = (n_channels_e==4) ? 3 : n_channels_e;
    for (int channel=0; channel!=nchannels; ++channel) {
        int v = int(wpix[channel]);
        wpix[channel] = lut[v];
    }
    return Pix(wpix);
}

template <class Pix, class BracketableRGBArray>
inline Pix applyLUT_RGB(Pix pix, BracketableRGBArray lut)
{
    typedef typename PixelTypes<Pix>::wrapped_pixel_type  wrapped_pixel_type;
    wrapped_pixel_type wpix = pix;
    enum { n_channels_e = PixelTypes<Pix>::n_channels_e };
    int nchannels = (n_channels_e==4) ? 3 : n_channels_e;
    for (int channel=0; channel!=nchannels; ++channel) {
        int v = int(wpix[channel]);
        wpix[channel] = lut[v][channel];
    }
    return Pix(wpix);
}

/// -------------------------------------------------------------------------------
// FORWARD DECLARATIONS FOR CLASS Img
/// -------------------------------------------------------------------------------

template<class Pix> class Img;

typedef Img<  signed  char>                 ImgChar;      // 1 byte
typedef Img<unsigned  char>                 ImgByte;      // 1 byte
typedef Img<         short>                 ImgShrt;      // 2 bytes short
typedef Img<unsigned short>                 ImgWord;      // 2 bytes unsigned short
typedef Img<bool>                           ImgBool;      // 1 bit
typedef Img<BoolTrue>                       ImgBoolTrue;  // 1 bit
typedef Img<int>                            ImgLong;      // 4 bytes int
typedef Img<unsigned int>                   ImgDwrd;      // 4 bytes unsigned int (Double WoRD)
typedef Img<long long>                      ImgKwrd;      // 8 bytes unsigned int (Quadro WoRD)
typedef Img<unsigned long long>             ImgQwrd;      // 8 bytes unsigned int (Quadro WoRD)
typedef Img<float>                          ImgReal;      // 4 bytes float
typedef Img<double>                         ImgDblp;      // 8 bytes DouBLe Precision
// -------------------------------------------------------------------------------
//typedef Img<NibblePixel<UnsignedPixel> >    ImgNibl;  // 4 bit
//typedef Img<NibblePixel<  SignedPixel> >    ImgSNib;  // 4 bit
typedef Img<ARGBpixel>                      ImgARGB;       //32  bit (4 bytes)
typedef Img< RGBpixel>                      Img_RGB;       //24  bit (3 bytes)
typedef Img<HRGBpixel>                      ImgHRGB;       //48  bit (3 shorts)
typedef Img<WRGBpixel>                      ImgWRGB;       //48  bit (3 words)
typedef Img<IRGBpixel>                      ImgIRGB;       //96  bit (3 ints)
typedef Img<KRGBpixel>                      ImgKRGB;       //192 bit (3 quads)
typedef Img<QRGBpixel>                      ImgQRGB;       //192 bit (3 quads)
typedef Img<FRGBpixel>                      ImgFRGB;       //96  bit (3 floats)
typedef Img<DRGBpixel>                      ImgDRGB;       //192 bit (3 doubles)

template<class DoublyBracketable> Img<typename DoublyBracketable::Pixel>   operator~(const DoublyBracketable& img); // forward declaration

///
// TEMPLATE FOR ALL DEPENDANT TYPES OF IMAGES
///

template<class Pix>
class ImgTypes
{
public:
    
    typedef typename PixelTypes<Pix>::value_type         value_type ;
    typedef typename PixelTypes<Pix>::      Pixel              Pixel      ;
    
    typedef typename PixelTypes<Pix>::ConstValue_typePtr   ConstValue_typePtr;
    typedef typename PixelTypes<Pix>::     Value_typePtr        Value_typePtr;
    
    typedef typename PixelTypes<Pix>::ConstPixelPtr        ConstPixelPtr   ;
    typedef typename PixelTypes<Pix>::     PixelPtr             PixelPtr   ;
    typedef typename PixelTypes<Pix>::ConstPixelRef        ConstPixelRef   ;
    typedef typename PixelTypes<Pix>::     PixelRef             PixelRef   ;
    
    typedef Img<Pix>                               FlatImg    ;
    
    enum {n_channels_e = PixelTypes<Pix>::n_channels_e };
    typedef typename PixelTypes<Pix>::PixBox         PixBox;
    
    //  // PixBox normally contains 1 pixel ( in bool   case - 32 pixels
    //  //                                  , in nibble case -  2 pixels)
    //
    enum {pixelsInBox_e = PixelTypes<Pix>::pixelsInBox_e };
};

///
// FORWARD DECLARATIONS OF 3 GLOBAL FUNCTIONS:
// getHeight()   getWidth()   getBits()
///
template<class PixPtr> inline int imgPixelBits(const PixPtr& p)    { return int(sizeof(*p))*8;}
template<>             inline int imgPixelBits(const BitPtr& p)    { return 1;}
//
//template<>  inline
//int imgPixelBits(const NibPtr<UnsignedPixel>& p)    { return 4;}
//
//template<>  inline
//int imgPixelBits(const NibPtr<  SignedPixel>& p)    { return 4;}

template<class Pix> class Img;

template<class Pix> inline int    getHeight(const Img<Pix>& p);
template<class Pix> inline int    getWidth (const Img<Pix>& p);
template<class Pix> inline int    getBits  (const Img<Pix>& p);

template<class Pix> class ConstROI ;

template<class Pix> inline int    getHeight(const ConstROI<Pix>& p);
template<class Pix> inline int    getWidth (const ConstROI<Pix>& p);
template<class Pix> inline int    getBits  (const ConstROI<Pix>& p);

template<class Pix> class ROI;

template<class Pix> inline int    getHeight(const ROI<Pix>& p);
template<class Pix> inline int    getWidth (const ROI<Pix>& p);
template<class Pix> inline int    getBits  (const ROI<Pix>& p);

#endif
