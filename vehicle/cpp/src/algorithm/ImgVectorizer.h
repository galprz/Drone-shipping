/// \file  ImgVectorizer.h   \date  1-Jan-2004  \author H.Kreitman
/// \brief ImgVectorizer, ImgVectorizer0x,  ImgVectorizer0x0value vectorizes binary, zero-crossing & zero-value of zero-crossing images respectively
/// \details
/// =======================================================================
/// ImgVectorizer     ,   ImgVectorizer0x   ,  ImgVectorizer0x0value
/// for binary images     for zero-crossing    zero-value of zero-crossing
///
///
/// ImgVectorizer()               - empty c-tor to work with img2curves
///
/// -----------------------------------------------------------------------
/// For ZERO-crossing with SIGNED img use
///
/// ImgVectorizer0x (with the same interface)
///
/// For most popular formats : signed char, unsigned char, bool
///      (i.e. ImgChar, ImgByte, ImgBool)
///    ZERO-level selected at mid of range.
///
///   !!! Default zero-value is 0 for all pixel-types, but 'unsigned char'
///   !!! has default zero-value 128.
///
///   !!! It can be changed in constructor or by set-function:
///   Example 1:
///      ImgVectorizer0x iv(otherValue)
///   Example 2:
///      ImgVectorizer0x iv().set0value(otherValue);
/// -----------------------------------------------------------------------
/// !!! Below AnyImg 'img' must support expression:  img[rowNo]
///         for int rowNo, returning 'pointer' to appropriate row of img
///
/// !!! Below ContPixPtr 'rowPtr' must support expression: rowPtr[clmNo]
///         for int clmNo, returning 'pixel', which is convertible to bool
///         i.e. supports boolean expression: pixel ? 1 : 0
///         Pixels, which converted to 0, will be called BLACK pixels.
/// -----------------------------------------------------------------------
/// iv.set1stRowNo(firstRowNumber)     set initial y-coordinate in polygons, which will be
///                                    computed from the given image. Returns Self.
///                                    (default: starting Y-coordinate is 0)
///
/// iv.set0value(anyZeroValue)         (in case of unsigned char pixels, default value is 128)
///
/// iv.img2curves ( const AnyImg& img , PolygonContainer* usrPgons)
///                               - fill container of containers of some kind of
///                                 'points' (the only request to 'points'-type:
///                                  to be constructible from 2 numbers)
///
/// iv.img2curves ( const AnyImg& img , PolylinContainer* usrPlins , PolygonContainer* usrPgons)
///                               - fill 2 containers of containers of some kind of
///                                 'points' (the only request to 'points'-type:
///                                  to be constructible from 2 numbers)
///
/// iv.img2curves ( const AnyRunLenImg& img , PolygonContainer* usrPgons)
///
/// iv.img2curves ( const AnyRunLenImg& img , PolylinContainer* usrPlins , PolygonContainer* usrPgons)
///
/// !!! RunLength NOTE
///    The type AnyRunLenImg should be capable to compile expression img[rowNo]
///    which return const pointer to some integral type, which is sequence of run-length
///    values of BLACK/WHITE colors. Starting color assumed to be BLACK.
///
/// -----------------------------------------------------------------------
/// !!! All returned polygons/polylines will have the same orientation,
///     i.e. the LEFT bank of polygon/polyline is BLACK - so for "regular"
///     contours the polygon will be CW, for a "hole" the polygon will
///     be CCW (counter clockwise).
/// -----------------------------------------------------------------------
///
/// Row-By-Row mode:
/// ----------------
/// iv.setImgWidth(nCols)   - first set image width (returns Self)
///
/// Execute the following command for each row of the img:
///
/// iv.addRow ( ConstPixPtr rowPtr , PolygonContainer* usrPgons)
///
/// iv.addRow ( ConstPixPtr rowPtr , PolylinContainer* usrPlins , PolygonContainer* usrPgons)
///
/// !!! See RunLength NOTE above.
///
/// iv.addBoolRunRow ( const R* runsRowPtr , PolygonContainer* usrPgons)
///
/// iv.addBoolRunRow ( const R* runsRowPtr , PolylinContainer* usrPlins , PolygonContainer* usrPgons)
///
/// !!! all calls to addRow must be with the same number of arguments
///     i.e. all calls with 2 arguments or all calls with 3 arguments.
///
/// At end of all rows flush all internal points :
///
/// iv.flush ( PolygonContainer* usrPgons )
/// or
/// iv.flush ( PolylinContainer* usrPlins )
///
/// =======================================================================
///
/// ///////////////////////////////////////////////////////////////////////
///
/// Since std::list<T> allocates only 1 object, the special allocator is provided
/// It works MUCH faster than std::allocator<T>
/// std::list<T , ImgVectorizerFastAllocator1<T> > should be used
///
///
/// ///////////////////////////////////////////////////////////////////////

#ifndef IMGVECTORIZER_H
#define IMGVECTORIZER_H

#include <cassert>
#include <memory>
#include <limits>
#include <list>
#include <vector>

#include <stack>

#include <iomanip>

#include <utility>

#include <algorithm>

#include "Img.h"


struct ImgVectorizerFastAllocator1area
{
    ImgVectorizerFastAllocator1area()
    : m_stack()
    , m_mmbSz(0)
    { }
    
    std::stack<void*>  m_stack;
    size_t             m_mmbSz;
};

template<class T>
class ImgVectorizerFastAllocator1 : public std::allocator<T>
{
    friend std::ostream& operator<<(std::ostream& os, const ImgVectorizerFastAllocator1<T>& fa)
    {
        os << " mmbSz=" << fa.m_areaPtr->m_mmbSz
        ;
        return os;
    }
public:
    ImgVectorizerFastAllocator1area*  m_areaPtr;
    
    typedef std::allocator<T>       base;
    
    typedef typename base::value_type        value_type;
    typedef typename base::pointer           pointer;
    typedef typename base::reference         reference;
    typedef typename base::const_pointer     const_pointer;
    typedef typename base::size_type         size_type;
    typedef typename base::difference_type   difference_type;
    
    template<class U>
    struct rebind { typedef ImgVectorizerFastAllocator1<U> other;};
    
    // ------ inherited member-functions -------------
    
    //--pointer       address(reference v) const          { return &v;}
    //--const_pointer address(const_reference v) const    { return &v;}
    //--pointer allocate(size_type n, const void* hint)   { return allocate(n);}
    //--void construct(pointer p, const T& v)             { ::new(p) T(v);}
    //--void destroy(pointer p)                           { p->~T();}
    //--size_type max_size() const    {  size_type n = size_type(-1) / sizeof (T); return (0 < int(n) ? n : 1); }
    
    ImgVectorizerFastAllocator1(ImgVectorizerFastAllocator1area& areaRef)
    : base()
    , m_areaPtr(&areaRef)
    { }
    
#ifdef AA_NOT_SUPPORTED_REBIND_ALLOCATOR
    char* _Charalloc(size_type N) {   // override VC++6.0 specific function
        setMemberSize();
        char* p;
        if (m_areaPtr->m_stack.empty())
            p = base::_Charalloc(N);
        else
            p = (char*)topNpop();
        return p;
    }
    
    // define VC++6.0 specific  deallocation (with void* param)
    void deallocate(void* p, size_type s)         { deallocate ( (T*)(p) , 1);}
    
#else
    // can't be compiled in Visual 6.0
    ImgVectorizerFastAllocator1(const ImgVectorizerFastAllocator1<T>& other)
    : base(other)
    , m_areaPtr(other.m_areaPtr)
    {
    }
    
#endif
    
    template<class U>
    ImgVectorizerFastAllocator1(const ImgVectorizerFastAllocator1<U>& other)
    : base(other)
    , m_areaPtr(other.m_areaPtr)
    {
    }
    
    template<class U>
    ImgVectorizerFastAllocator1<T>& operator=(const ImgVectorizerFastAllocator1<U>& other)
    {
        m_areaPtr = other.m_areaPtr;
        return *this;
    }
    
    void deallocate(pointer p, size_type s)
    {
        assert (s==1);
        setMemberSize();
        if (sizeof(T) < m_areaPtr->m_mmbSz) {
            base::deallocate(p,1);      // don't save small chunks in stash
        }else
            m_areaPtr->m_stack.push(p);
    }
    
    pointer allocate(size_type N)
    {
        assert(N==1);
        setMemberSize();
        return  (m_areaPtr->m_stack.empty()) ? newArea() : topNpop();
    }
    
    void  clear()               // additional member-function
    {
        while (!m_areaPtr->m_stack.empty())
            base::deallocate ( topNpop() , 1);
    }
    
private:
    
    void setMemberSize()
    {
        if (m_areaPtr->m_mmbSz < sizeof(T)) {
            clear();
            m_areaPtr->m_mmbSz = sizeof(T);
        }
    }
    
    pointer topNpop() {
        pointer p = reinterpret_cast<pointer>(m_areaPtr->m_stack.top());
        m_areaPtr->m_stack.pop();
        return p;
    }
    
    pointer newArea() {
        const void* hint = 0;
        pointer p = base::allocate(1,hint);
        return p;
    }
    
};

template <class T, class U>
bool operator==(const ImgVectorizerFastAllocator1<T>& a,const ImgVectorizerFastAllocator1<U>& b) { return a.m_areaPtr==b.m_areaPtr;}

template <class T, class U>
bool operator!=(const ImgVectorizerFastAllocator1<T>& a,const ImgVectorizerFastAllocator1<U>& b) { return a.m_areaPtr!=b.m_areaPtr;}


/// ///////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//// InF<n> Real number: (sizeof(int)*8-n) bits int part, n bits fraction part
//////////////////////////////////////////////////////////////////////////

template <int n>
struct InF {
    
    typedef    int    T;
    
    enum { one_e = 1<<n
        , mask_e = one_e - 1
        , half_e = one_e >> 1
        , nan_e  = T(~(~0u >> 1))
    };
    
    friend std::ostream& operator<<(std::ostream& s, const InF<n>& v)
    {
        return s << std::fixed << std::setprecision(3) << float(v);
    }
    
    //  ~InF()                              // d-tor: generated by compiler
    
    InF()                               // default c-tor
#ifndef NDEBUG
    : value(nan_e) // in DEBUG version be deterministic BAD value
#endif
    {
    }
    
    explicit InF(  int   v) : value(v<<n)     { }  // c-tor
    explicit InF(float   v) : value(v<0 ? -roundPos(-v) : roundPos(v)) {}    // c-tor
    
    InF(int number,int divisor) : value( (((number<<(n+1)) / divisor)+1)>>1 )     { }  // c-tor
    
    operator      float () const    { return value/float(one_e);}
    
    int  asIntBits() const          { return int(value);}
    
    InF<n>& operator+=(InF v)       { value += v.value; return *this;}
    InF<n>& operator-=(InF v)       { value -= v.value; return *this;}
    
    InF<n>& operator<<=(int v)      { value <<= v; return *this;}
    InF<n>& operator>>=(int v)      { value >>= v; return *this;}
    
    InF<n>  operator- () const      { InF<n> v; v.value = -value; return v;}
    InF<n>  operator~ () const      { InF<n> v; v.value = ~value; return v;}
    
    bool operator< (InF<n> v) const { return value < v.value;}
    bool operator<=(InF<n> v) const { return value <=v.value;}
    bool operator> (InF<n> v) const { return value > v.value;}
    bool operator>=(InF<n> v) const { return value >=v.value;}
    bool operator==(InF<n> v) const { return value ==v.value;}
    bool operator!=(InF<n> v) const { return value !=v.value;}
    
    InF<n> abs() const              { InF<n> v; v.value = ::abs(value); return v;}
    bool   isInt() const            { return (value&mask_e)==0;}
    
    static bool sameSign(const InF<n>& a,const InF<n>& b) { return (a.value^b.value) >= 0;}
    
    static InF<n> epsilon()         { InF<n> a(0); a.value=1; return a;}
    
private:
    T value;
    
    static int roundPos(float v) { return int(v*float(one_e)+0.5f);}
};

template<int n> inline InF<n> operator+(InF<n> a,InF<n> b)  { return a+=b;}
template<int n> inline InF<n> operator-(InF<n> a,InF<n> b)  { return a-=b;}

template<int n> inline InF<n> operator<<(InF<n> a,int b)    { return a<<=b;}
template<int n> inline InF<n> operator>>(InF<n> a,int b)    { return a>>=b;}

////////////////////////////////////////////////////////////////////////////

template <class List2dPoints>
struct PlinSprout {     // REFERENCE SEMANTICS
    typedef List2dPoints                           Plin;
    
    typedef typename List2dPoints::value_type      Point2d;
    //    typedef typename List2dPoints::allocator_type  ListPoint2dAllocator;
    
    typedef typename Point2d::first_type           Coord;
    
private:
    Plin*  plinPtr;
    bool   beg;                // WHITE is on the RIGHT side
public:
    PlinSprout(Plin* p , bool beg) : plinPtr(p) , beg(beg)   { }
    
    //  PlinSprout(const PlinSprout&);             // generated by compiler
    //  PlinSprout& operator=(const PlinSprout&);  // generated by compiler
    
    int leftSproutClr() const { return beg ? 1 : 0;}
    //  int rghtSproutClr() const { return beg ? 0 : 1;}
    
    const Point2d& thisEdgeRef() const { return  beg ? plinPtr->front() : plinPtr->back();}
    Point2d& thisEdgeRef()       { return  beg ? plinPtr->front() : plinPtr->back();}
    
    const Point2d& scndEdgeRef() const { return !beg ? plinPtr->front() : plinPtr->back();}
    Point2d& scndEdgeRef()       { return !beg ? plinPtr->front() : plinPtr->back();}
    
    Coord thisVertexX() const { return thisEdgeRef().first ;}
    Coord thisVertexY() const { return thisEdgeRef().second;}
    
    Coord scndVertexX() const { return scndEdgeRef().first ;}
    //  Coord scndVertexY() const { return scndEdgeRef().second;}
    
    void  remapto(PlinSprout& other)       { plinPtr = other.plinPtr;}
    
    const void* sproutVoidPtr() const      { return plinPtr;}
    
    const Plin&  polyline() const          { return *plinPtr;}
    Plin&  polyline()                { return *plinPtr;}
    
    bool   empty() const                   { return plinPtr->empty();}
    
    bool   kind() const                    { return beg;}
    
    void add(const Point2d& p) {
        typename Plin::iterator it = beg ? plinPtr->begin() : plinPtr->end();
        plinPtr->insert ( it , p );
    }
    
    void moveFrom(PlinSprout& other) {
        assert(beg != other.beg);
        // void splice(iterator position, list<T,Allocator>& x);
        //  Effects:
        //    Inserts the contents of x before position and x becomes empty.
        if (beg) other.plinPtr->splice(other.plinPtr->end() , *plinPtr);
        
        plinPtr->splice(plinPtr->end() , *other.plinPtr);
    }
    
    bool operator < ( const PlinSprout& rhs ) const { return thisVertexX() < rhs.thisVertexX();}
};

//////////////////////////////////////////////////////////////////////////

// returns the number of runs

template <class ConstPixPtr>
int _bin2rleSubst ( ConstPixPtr row, int* rlePtr , int clms )
{
    // very critical loop !!! BitPtr case !!!
    int  counter = 0;
    int  n = 0;
    for (int c=0; c!=clms; ++c) {
        int pixClr = row[c] ? 1 : 0;
        int runClr = n%2;
        if (pixClr!=runClr) {
            rlePtr[n++] = counter;
            counter = 0;
        }
        ++counter;
    }
    
    rlePtr[n] = counter;
    return n+1;
}

template <> inline
int _bin2rleSubst ( BitPtr row, int* rlePtr , int clms )
{
    return int(pack2runs(row, rlePtr , clms ));
}

template <class ConstPixPtr>
int _bin2rle ( ConstPixPtr row, int* rlePtr , int clms )
{
    return _bin2rleSubst(row,rlePtr,clms);
}

// returns the number of runs
template <class R>
int _rle2rle ( const R* inRunsPtr, int* rlePtr , int clms )
{
    int n   = 0;
    int cpyClms = clms;
    for (; cpyClms > 0; cpyClms -= (rlePtr[n++] = *inRunsPtr++))
    {
    }
    
    assert (cpyClms == 0);
    if (cpyClms < 0) {
        rlePtr[n-1] += cpyClms;  // neutralize BAD number of clms
        throw std::runtime_error("# of clms in given row is wrong");
    }
    
    return n;
}

template <class ConstPixPtr>
struct   _FuncDefiner
{
    typedef int (*Ptr2func)(ConstPixPtr, int* , int);
    
    template<class Pix>
    static Ptr2func get_toRLEfunc(Pix)
    {
        Ptr2func toRLEfunc = &_bin2rle;
        return toRLEfunc;
    }
};

template <>
struct   _FuncDefiner<const void*>
{
    template <class R>
    struct FuncNamer
    {
        typedef int (*Ptr2func)(const R* , int* , int);
    };
    
    template<class R>
    static typename FuncNamer<R>::Ptr2func  get_toRLEfunc(R)
    {
        typename FuncNamer<R>::Ptr2func toRLEfunc = &_rle2rle;
        return toRLEfunc;
    }
};

class ImgVectorizer;
class ImgVectorizer0x;

template <class CoordType>
struct ImgVectorizerBase
{
private:
    friend class ImgVectorizer;
    friend class ImgVectorizer0x;
    
    static bool is0x() {
        static CoordType half = halfCell();
        return !(half==CoordType(0));
    }
    
public:
    typedef CoordType                                Coord;
    typedef std::pair<Coord , Coord >                Point2d;
    
    //    typedef std::ListAllocator<Point2d>              ListPoint2dAllocator;
    typedef ImgVectorizerFastAllocator1<Point2d>     ListPoint2dAllocator;
    
    typedef std::list<Point2d,ListPoint2dAllocator>  Plin;
    typedef std::list <Plin>                         Plins;
    //    typedef std::vector <Plin>                       Plins;
    typedef PlinSprout<Plin>                         Sprout;
    typedef std::vector< Sprout >                    Sprouts;
    
    typedef ImgVectorizerBase<CoordType>             Base;
    
    typedef typename Sprouts::iterator               SproutsIterator;
    typedef typename Sprouts::const_iterator         SproutsConstIterator;
    
    typedef typename Plins::iterator                 PlinsIterator;
    typedef typename Plins::const_iterator           PlinsConstIterator;
    
    static CoordType halfCell()            { return CoordType(1) >> 1; }
    
public:
    int                             m_rowNo;
    int                             m_clms;
    ImgVectorizerFastAllocator1area m_stack;
    ListPoint2dAllocator            m_allocator;
    Plins                           m_plins;
    int                             m_ext;     // extend rows by 0 or 1 pix on both sides
    int                             m_clrUnder;// relevant only in case with only 1 run in prv row
    
private:
    
    static void printPoint2d(std::ostream& os , Point2d p) { os << p.first << " " << p.second;}
    
    ~ImgVectorizerBase ()
    {
        m_allocator.clear();
    }
    
    ImgVectorizerBase ()        // c-tor
    : m_rowNo (-1)          // set lowest Y of polygons to be 0
    , m_clms(0)             // will be set from img2curves of by setImgWidth
    , m_stack()
    , m_allocator(m_stack)
    , m_plins()
    , m_ext(-1)             // assume undef
    , m_clrUnder(0)
    {
    }
    
public:
    
    ImgVectorizerBase (const ImgVectorizerBase<CoordType>& rhs)                      // dflt c-tor
    : m_rowNo    ( rhs.m_rowNo     )
    , m_clms     ( rhs.m_clms      )
    , m_allocator( rhs.m_allocator )
    , m_plins    ( rhs.m_plins     )
    , m_ext      ( rhs.m_ext       )
    , m_clrUnder ( rhs.m_clrUnder  )
    {
    }
    
    void setImgWidth(int clms)           { m_clms = clms;}
    
    void set1stRowNo(int firstRowNo)     { m_rowNo = firstRowNo-1;}
    
    ImgVectorizerBase<CoordType>& operator=(const ImgVectorizerBase<CoordType>& rhs)
    {
        if (this != &rhs) {
            this->~ImgVectorizerBase();
            std::allocator<ImgVectorizerBase<CoordType> >().construct(this,rhs);
        }
        return *this;
    }
    
    template <class PolyContainer>
    void moveto(Plin& plin , PolyContainer* usrCurves)
    {
        typedef typename PolyContainer::value_type  Pcurve;
        
        usrCurves->insert(usrCurves->end() , Pcurve() );
        Pcurve& usrPcurve = usrCurves->back();
        
        typedef typename Pcurve::value_type        PointXY;
        
        Coord extCorrection = is0x() ? Coord(0) : -Coord(m_ext);
        
        for (typename Plin::const_iterator it = plin.begin(); !(it == plin.end()); ++it )
            usrPcurve.insert(usrPcurve.end(), PointXY (it->first + extCorrection , it->second) );
        plin.clear();
    }
    
    Coord sproutX(const Sprouts& sprouts, SproutsIterator itSprout)
    {
        return itSprout==sprouts.end() ? Coord(clmsExt()) : itSprout->thisVertexX();
    }
    
    int clmsExt() const       { return int(m_clms+m_ext*2);}
    
    template < class PolycrvContainer>
    void createSortedSprouts(Sprouts& sprouts , PolycrvContainer* usrPcrvs) {
        Coord maxy(0);
        Coord cury = Coord(m_rowNo) - halfCell();
        
        for (PlinsIterator it = m_plins.begin(); !(it==m_plins.end() ); ++it )
        {
            Plin& plin = *it;
            if (plin.size()==0)
                continue;
            assert (plin.size() > 1);
            size_t nPlinSprouts = 0;
            
            for (int i=0; i!=2; ++i) {  // loop : beg-to-end , end-to-beg
                bool beg = (i==0);
                Sprout sprout( &plin , beg );      // start/finish point
                Coord y = sprout.thisVertexY();
                if (! (m_ext || (m_ext==0 && (y==Coord(0) || y==(Coord(m_rowNo) - halfCell() ) || sprout.thisVertexX()==Coord(0) || sprout.thisVertexX() == Coord(clmsExt()) ) )) ){
                    return;
                }
                if (maxy < y)
                    maxy = y;
                
                if (y==cury)
                    ++nPlinSprouts, sprouts.insert ( sprouts.end() , sprout );
            }
            if (nPlinSprouts==0) {          // i.e. finished polycurves
                moveto ( plin , usrPcrvs ); // move from plin to usrPcrvs
            }
        }
        
        if (sprouts.size()==0) {
            // all internal curves were finished, move them to usrPcrvs
            for (PlinsIterator it = m_plins.begin(); !(it==m_plins.end() ); ++it ) {
                Plin& plin = *it;
                if (plin.size())
                    moveto ( plin , usrPcrvs );
            }
            m_plins.clear();
        }
        std::sort ( sprouts.begin() , sprouts.end());
    }
    
    template < class PolylinContainer>
    void  setExt(PolylinContainer* usrPlins)
    {
        if (m_clms==0) {
            assert(("ImgVectorizer::setExt: m_clms==0 in c-tor"-"")*0);
            //            assert(("ImgVectorizer::setExt: m_clms==0 in c-tor" ,false));
            throw std::logic_error("parameter clms should be != 0 in constructor of ImgVectorizer");
        }
        
        if((usrPlins==0 && m_ext==0)
           || (usrPlins!=0 && m_ext==1)) {
            //            assert("ImgVectorizer::setExt: usrPolylines should be zero/non-zero for all rows",false));
            assert(("ImgVectorizer::setExt: usrPolylines should be zero/non-zero for all rows"-"")*0);
            throw std::logic_error("usrPolylines should be zero/non-zero for all rows");
        }
        if (m_ext!=0 && m_ext!=1)
            m_ext = (usrPlins==0) ? 1 : 0;
    }
    
    static bool isZeroPlin (const Plin& p)  { return p.size()==0;}
    
    void addNewPlins ( Plins& newPlins )
    {
        // kill empty polylines
        m_plins.erase ( std::remove_if(m_plins.begin(), m_plins.end(), isZeroPlin ) , m_plins.end() );
        // now add all new polylines
        m_plins.splice ( m_plins.end() , newPlins );
    }
    
    // ck x-sorted sprouts   for assert/DBG only
    bool goodSprouts(const Sprouts& sprouts) {
        if (sprouts.size()) {
            for (size_t i=1; i!=sprouts.size(); ++i)
                if ( sprouts[i].kind() == sprouts[i-1].kind()
                    || sprouts[i].thisVertexX() <= sprouts[i-1].thisVertexX() )
                    return false;
        }
        return true;
    }
    
    template <class PolygonContainer>
    void connect ( SproutsIterator itConnSprout ,SproutsIterator itSprout
                  , Sprouts& sprouts , PolygonContainer* usrPgons)
    {
        if ( itConnSprout->sproutVoidPtr()==itSprout->sproutVoidPtr() )
            moveto ( itConnSprout->polyline() , usrPgons );
        //      __________
        //     |___       |__________
        //         |_________________|
        //
        else
            mergePlins ( itConnSprout , itSprout , sprouts );
        //      __________
        //     |___       |__________
        //         |_              __|
        //           |            |
    }
    
    void mergePlins ( SproutsIterator itConnSprout,SproutsIterator itSprout
                     , Sprouts& sprouts)
    {
        //
        //    |    |   |___          ____|
        //    |_   |_______|     ___|
        //      |_______________|
        //
        itConnSprout->moveFrom(*itSprout);
        
        const void* sprout2kill = itSprout->sproutVoidPtr();
        for (SproutsIterator it = itSprout; !(it==sprouts.end()); ++it)
            if (it->sproutVoidPtr()== sprout2kill)
                it->remapto(*itConnSprout);
        
    }
    
protected:
    
    static std::ostream& printPlins(std::ostream& s , const Plins& plins)
    {
        size_t plinNo = 0;
        for (typename Plins::const_iterator i=plins.begin(); i!=plins.end(); ++i, ++plinNo)
        {
            s << '[' << plinNo << "]";
            const Plin& plin = *i;
            s << '(' << plin.size() << ')';
            if (plin.size()) {
                printPoint2d(s , plin.front());
                bool printBack = true;
                if (plin.size() > 2) {
                    if (plin.size() > 5)
                        s << " ... ";
                    else {
                        printBack = false;
                        typename Plin::const_iterator j = plin.begin();
                        for (++j; j!=plin.end(); ++j) {
                            s << " ";
                            printPoint2d(s , *j);
                        }
                    }
                }else
                    s << " ";
                if (printBack)
                    printPoint2d(s , plin.back());
                
            }
            s << std::endl;
        }
        return s;
    }
    
    static std::ostream& printSprouts(std::ostream& s , const Sprouts& sprouts)
    {
        SproutsConstIterator it = sprouts.begin();
        for (size_t i=0; i!=sprouts.size(); ++i,++it )
        {
            s << '[' << i << "]";
            const Sprout& sprout = *it;
            
            if(sprout.sproutVoidPtr()==0) {
                s << " EMPTY";
            }else {
                s << sprout.polyline().size() << " points. ";
                if (sprout.polyline().size()) {
                    s << "from ";
                    printPoint2d(s , sprout.thisEdgeRef());
                    s << " to ";
                    printPoint2d(s , sprout.scndEdgeRef());
                }
            }
            
            s << std::endl;
        }
        return s;
    }
};

class ImgVectorizer : public ImgVectorizerBase<int>
{
    //--typedef int                                      Coord;
    //--typedef std::pair<Coord , Coord >                Point2d;
    //--typedef ImgVectorizerFastAllocator1<Point2d>     ListPoint2dAllocator;
    //--typedef std::list<Point2d,ListPoint2dAllocator>  Plin;
    //--typedef PlinSprout<Coord>                        Sprout;
    //--typedef std::vector< Sprout >                    Sprouts;
    //--typedef std::list <Plin>                         Plins;
    //--typedef ImgVectorizerBase<CoordType>             Base;
    //--typedef Sprouts::iterator                        SproutsIterator;
    //--typedef Sprouts::const_iterator                  SproutsConstIterator;
    //--typedef Plins::iterator                          PlinsIterator;
    //--typedef Plins::const_iterator                    PlinsConstIterator;
    
    //
    // segments should be DIRECTED segments (white on the LEFT side)
    
    friend std::ostream& operator<< ( std::ostream& s , const ImgVectorizer& iv) {
        s << std::endl << "rowNo=" << iv.m_rowNo << " clms=" << iv.m_clms << " clrUnder=" << iv.m_clrUnder << " runs:";
        const int* m_runs = iv.m_runs;
        for (int c=0; c<iv.m_clms; c += *m_runs++) s << std::setw(6) << *m_runs;
        
        for (PlinsConstIterator it=iv.m_plins.begin(); !(it==iv.m_plins.end()); ++it)
        {
            s << std::endl;
            const Plin& plin = *it;
            for (Plin::const_iterator i = plin.begin(); !(i==plin.end()); ++i )
                s << (i==plin.begin() ? "" : ",  ") << std::setw(6) << i->first << std::setw(6) << i->second;
            
            s << ";" << std::endl;
        }
        
        return s;
    }
    
public:
    typedef     ImgVectorizer                   Self;
    
    
    ~ImgVectorizer()         { delete[] m_runs; }
    
    ImgVectorizer ()   // c-tor
    : Base()
    , m_runs(0)
    {
    }
    
    ImgVectorizer(const ImgVectorizer& rhs)       // copy c-tor
    : Base(rhs)
    , m_runs  ( (rhs.m_runs ? (new int[m_clms+2]) : 0) )
    {
        if (m_runs && rhs.m_runs)
            memcpy(m_runs , rhs.m_runs, (m_clms+2)*sizeof(int));
    }
    
    void operator=(const ImgVectorizer& rhs) {
        if (this != &rhs) {
            this->~ImgVectorizer();
            std::allocator<ImgVectorizer>().construct(this,rhs);
        }
    }
    
    Self& setImgWidth(int clms)           { Base::setImgWidth(clms);        return *this;}
    Self& set1stRowNo(int firstRowNo)     { Base::set1stRowNo(firstRowNo);  return *this;}
    
    template < class AnyImg , class PolygonContainer >
    void img2curves ( const AnyImg& img, PolygonContainer* usrPgons )
    {
        img2curves(img, (PolygonContainer*)0, usrPgons);
    }
    
    template < class AnyImg , class PolylinContainer , class PolygonContainer >
    void img2curves ( const AnyImg& img
                     , PolylinContainer* usrPlins
                     , PolygonContainer* usrPgons
                     )
    {
        m_clms = getWidth (img);
        delete[] m_runs;
        m_runs = 0;
        m_rowNo = -1;
        m_plins.clear();
        m_clrUnder = 0;
        
        int rows = getHeight(img);
        for (int r=0; r!=rows; ++r)
        {
            typedef typename AnyImg::ConstPixelPtr   ConstPixelPtr;
            typedef typename AnyImg::value_type      value_type;
            addAnyRow ( img[r] , usrPlins , usrPgons
                       , _FuncDefiner<ConstPixelPtr>::get_toRLEfunc(value_type(0) ));
        }
        
        if (usrPlins)
            flush(usrPlins);
        else
            flush(usrPgons);
        
        //        privReverse(usrPlins);
        //        privReverse(usrPgons);
    }
    
    template < class PolycrvContainer >
    void flush ( PolycrvContainer* usrPcrvs )
    {
        if (m_ext==0) {
            // poly-curve is polyline
            while (!m_plins.empty()) {
                moveto ( m_plins.front() , usrPcrvs );
                m_plins.erase(m_plins.begin());
            }
        }else if(m_ext==1) {
            // poly-curve is polygon
            m_runs[0] = int(m_clms)+2; // create dummy runsPtr representing empty row
            ++m_rowNo;
            addRuns( 1 , (PolycrvContainer*)0 , usrPcrvs);
        }
    }
    
    template < class R , class PolygonContainer >
    void addBoolRunRow( const R* constRunsPtr
                       , PolygonContainer* usrPgons
                       )
    {
        addBoolRunRow(constRunsPtr,(PolygonContainer*)0,usrPgons);
    }
    
    template < class R , class PolylinContainer , class PolygonContainer >
    void addBoolRunRow ( const R* runsPtr
                        , PolylinContainer* usrPlins
                        , PolygonContainer* usrPgons
                        )
    {
        typedef int (*Func)(const R*, int* , int);
        
        Func toRLEfunc = &_rle2rle;
        
        addAnyRow(runsPtr,usrPlins,usrPgons,toRLEfunc);
    }
    
    template < class ConstPixPtr , class PolygonContainer >
    void addRow ( ConstPixPtr currRow , PolygonContainer* usrPgons)
    {
        addRow(currRow , (PolygonContainer*)0, usrPgons);
    }
    
    template < class ConstPixPtr , class PolylinContainer , class PolygonContainer >
    void addRow ( ConstPixPtr currRow
                 , PolylinContainer* usrPlins
                 , PolygonContainer* usrPgons
                 )
    {
        typedef int (*Func)(ConstPixPtr, int* , int);
        
        Func toRLEfunc = &_bin2rle;
        
        addAnyRow(currRow,usrPlins,usrPgons,toRLEfunc);
    }
    
private:
    //--int                   m_rowNo;
    //--size_t                m_clms;
    //--ListPoint2dAllocator  m_allocator;
    //--Plins                 m_plins;
    //--int                   m_ext;     // extend rows by 0 or 1 pix on both sides
    //--int                   m_clrUnder;// relevant only in case with only 1 run in prv row
    int*                  m_runs;
    
    template < class ConstPixPtr , class PolylinContainer , class PolygonContainer  , class Func>
    void addAnyRow ( ConstPixPtr currRow
                    , PolylinContainer* usrPlins
                    , PolygonContainer* usrPgons
                    , Func  toRLEfunc
                    )
    {
        setExt(usrPlins);
        
        bool empty = (m_runs==0);   // still no rows added
        ++m_rowNo;
        
        if (m_runs==0)
            m_runs = new int[m_clms+2];
        
        int nRuns = int(toRLEfunc ( currRow, m_runs , int(m_clms)));
        if (m_ext) {
            --nRuns;                   // put it to be actual index
            m_runs[0] += m_ext;        // but 1st expand first (black) run
            if (nRuns%2 == 0 && m_ext) // i.e. last run has black color
                m_runs[nRuns] += m_ext; // expand it
            else
                m_runs[++nRuns] = m_ext;// add new run with run == 1
            ++nRuns;                   // back to being real number of runs
        }
        
        if (empty && m_ext==0) {
            // special case for 1st row : use only runs
            assert (usrPgons->size()==0);
            assert (usrPlins->size()==0);
            addPreRow();   // i.e create VERTICAL sprouts from runs
            return;
        }
        
        addRuns ( nRuns , usrPlins , usrPgons);
    }
    
    void addPreRow ()  // create VERTICAL sprouts from runs
    {
        assert ( m_plins. size()==0);
        
        int i = 0;
        if (m_runs[0]==0)   // starts from WHITE clr
            ++i;
        
        m_clrUnder = i%2;        // relevant only in case with only 1 run
        int clm = m_runs[i];  // x-coordinate of RGHT side of run
        for ( ;clm != int(clmsExt()); clm += m_runs[++i]) {
            assert (clm < int(clmsExt()));
            // create STICK on the rght side of current run
            m_plins.insert(m_plins.end() , Plin( 2 , Point2d(clm,m_rowNo) , m_allocator ) );
            
            Plin& vertPlin = m_plins.back();      // vertical stick (but still y0==y1)
            Plin::iterator p = vertPlin.begin();// pointer to vertex 0
            
            int clr = i%2;
            if (clr==0)
                ++p;                               // pointer to vertex 1
            
            ++p->second;                           // make y0 (or y1) to be 1
        }
    }
    
    template < class PolylinContainer , class PolygonContainer >
    void addRuns ( int nRuns
                  , PolylinContainer* usrPlins
                  , PolygonContainer* usrPgons
                  )
    {
        // now work with rowNo and runs
        
        // -----------------------------------------------------------------
        // normal case : we have curr row and sprouts from prev polylines
        // -----------------------------------------------------------------
        
        // create sorted array of polylin sprouts (from plins),
        // moving finished polylines into usrPlins
        
        Sprouts sprouts;
        
        if (usrPlins)
            createSortedSprouts(sprouts , usrPlins);
        else
            createSortedSprouts(sprouts , usrPgons);
        
        if (sprouts.empty())
        {
            createNewPlins();
            return;
        }
        
        assert ( goodSprouts(sprouts) );
        
        // -----------------------------------
        //  ___________________________________
        // |_____________|______|________|_____|  runs
        //      |       |       |     |     |     x-sorted sprouts
        //
        
        Plins newPlins;
        
        int clm = 0;
        
        int xPrevSprout = 0;                             // 1st loop variable
        SproutsIterator itPrevSprout = sprouts.end();    // 2nd loop variable
        
        SproutsIterator itSprout = sprouts.begin();      // 3rd loop variable
        int xSprout=sproutX(sprouts, itSprout);          // 4th loop variable
        
        int sproutLeftClr = itSprout->leftSproutClr();   // 5th loop variable
        
        for (int i = (m_runs[0] ? 0 : 1); i != nRuns; clm += m_runs[i++]) {
            // all sprouts with x <= clm, already passed
            int run = m_runs[i];
            int clr = i%2;
            
            int clmnxt = clm+run;
            
            // pass all sprouts in this run (including xSprout==clmnxt)
            
            for ( ; ;//xSprout <= clmnxt && xSprout != clmsExt();
                 )
            {
                if (sproutLeftClr==clr) {
                    // THE SAME COLOR
                    //   |xxxxxxxx|  or    |xxxxxx|
                    //    xxxx|                xxx|
                    //        ^                   ^
                    if (xSprout==clmnxt && clmnxt != clmsExt())
                        itSprout->thisEdgeRef().second = m_rowNo + 1;
                    
                }else {
                    // clm______________clmnxt
                    //   |______________|       runs  (curr row)
                    // |||||          |||||     x-sorted sprouts(prev row)
                    // xPrevSprout   xSprout  (xPrevSprout can be <,> or == clm)
                    //                        (xSprout     can be <,> or == clmnxt)
                    //
                    // common horizontal non-empty segment:
                    // [ max(xPrevSprout,clm)  min(xSprout,clmnxt) ]
                    //
                    // common horizontal segment has left vertical item
                    // either leftL(on upper row) or leftG(on lower row)
                    // (G means Greek capital letter Gamma)
                    //
                    // common horizontal segment has rght vertical item
                    // either rghtL(on upper row) ot rghtG(on lower row)
                    enum LeftKind {leftL_e=0 , leftG_e=1};
                    //  |xxxxx           |xxx       |000         |111        ||000        ||111
                    //    *---           *---       *---         *---         *---         *---
                    //    |yyy         |yyy         |111         |000        ||111        ||000
                    //    ^            ^            ^            ^            ^            ^
                    // xPrevSprout  xPrevSprout  xPrevSprout  xPrevSprout  xPrevSprout  xPrevSprout
                    //  leftG        leftL        leftL        leftG        leftL        leftL
                    // ----------------------------------------------------------------------------
                    LeftKind left = leftL_e;
                    if(clm < xPrevSprout || (clm==xPrevSprout && clr && clm!=0))
                        left = leftG_e;
                    
                    enum RghtKind {rghtL_e=0 , rghtG_e=1};
                    // ----------------------------------------------------------------------------
                    //  xxxx|    xxx|           000|       111|        000||      000||
                    // ---*      ---*           ---*       ---*        ---*       ---*
                    // yyy|        yyy|         111|       000|        111||      111||
                    //    ^           ^            ^          ^           ^          ^
                    // xSprout     xSprout      xSprout    xSprout     xSprout    xSprout
                    //  rghtG     rghtL          rghtL      rghtG       rghtL      rghtL
                    // ----------------------------------------------------------------------------
                    RghtKind rght = rghtL_e;
                    if(clmnxt > xSprout || (clmnxt==xSprout && clr && clmnxt!=clmsExt()))
                        rght = rghtG_e;
                    
                    if(left==leftL_e) {
                        if (rght==rghtL_e)
                            openNew(clm , clmnxt , clr , newPlins);
                        else    // rght==rghtG_e
                            continueSprout ( itSprout , clm );
                    }else {     // left==leftG_e
                        if (rght==rghtL_e)
                            continueSprout ( itPrevSprout , clmnxt );
                        else    // rght==rghtG_e
                            connect ( itPrevSprout , itSprout , sprouts , usrPgons );
                    }
                }
                // ck loop conditions
                if (xSprout > clmnxt || xSprout==clmsExt())
                    break;
                // bump 5 loop variables
                (xPrevSprout=xSprout)
                ,(sproutLeftClr = 1-sproutLeftClr)
                , itPrevSprout = itSprout
                ,(xSprout=sproutX(sprouts, ++itSprout))
                ;
                // ck again loop conditions
                if (xPrevSprout>=clmnxt)
                    break;
            }
        }
        assert (clm == clmsExt());
        //        killEmptyPlins();
        addNewPlins ( newPlins );
        
        if ( m_runs[nRuns-1]==clmsExt())
            m_clrUnder = (nRuns-1)%2;
    }
    
    //     template < class PolycrvContainer >
    //     static void privReverse ( PolycrvContainer* usrPcrvs ) {
    //         // reverse direction of all curves
    //         typedef typename PolycrvContainer::iterator  CurvesIterator;
    //         if (usrPcrvs)
    //             for (CurvesIterator it = usrPcrvs->begin(); !(it==usrPcrvs->end()); ++it)
    //                 std::reverse(it->begin() , it->end());
    //     }
    
    void openNew(int xbeg , int xend , int clr , Plins& anyPlins) {
        Plin plin(m_allocator);
        Plins::iterator iter = anyPlins.insert ( anyPlins.end() , plin);
        Plin& plinRef = *iter;
        if (xbeg)
            plinRef.push_back(Point2d (xbeg,m_rowNo+1));
        plinRef.push_back(Point2d (xbeg,m_rowNo));
        plinRef.push_back(Point2d (xend,m_rowNo));
        if (xend!=clmsExt())
            plinRef.push_back(Point2d (xend,m_rowNo+1));
        
        if (clr!=0) {
            std::swap ( plinRef.front() , plinRef.back());
            if (plinRef.size()==4) {
                Plin::iterator i1 = plinRef.begin();  ++i1;
                Plin::iterator i2 = i1;            ++i2;
                std::swap(*i1 , *i2);
            }
        }
    }
    
    //     int leftColor(const Sprouts& sprouts, SproutsIterator itSprout) {
    //         int leftclr  = m_clrUnder;
    //         if (itSprout == sprouts.end()) {
    //              if (!sprouts.empty()) {
    //                  SproutsIterator itPrev = itSprout;
    //                  --itPrev;
    //                  leftclr = itPrev->rghtSproutClr();
    //              }
    //         }else
    //              leftclr  = itSprout->leftSproutClr();
    //         return leftclr;
    //     }
    
    void createNewPlins()
    {
        assert (m_plins.size()==0);
        int runIndex = m_runs[0] ? 0 : 1;
        int clr = 1 - m_clrUnder;
        for (int clm=0; clm!=clmsExt(); clm += m_runs[runIndex++]) {
            if (runIndex%2 != m_clrUnder)
                openNew ( clm , clm + m_runs[runIndex] , clr , m_plins );
        }
    }
    
    void continueSprout(SproutsIterator itSprout , int x)
    {
        itSprout->add(Point2d(x , m_rowNo));
        if (x && x != clmsExt())
            itSprout->add(Point2d(x , m_rowNo+1));
    }
};

// ----------------------------------------------------------------------------------

template <class Pix> inline
int _getZeroValue(Pix*) { return 0;}

template <> inline
int _getZeroValue(unsigned char*)  { return 128;}

//struct ImgVectorizer0x0valueBase { typedef InF<4> PixVal;};
//
//template <class Pix>
//struct ImgVectorizer0x0value : public ImgVectorizer0x0valueBase {
////--typedef InF<4>                 PixVal;
//    static PixVal zeroValue;
//
//    template<class Pix2>
//    static void   set0value(Pix2 v)    {        zeroValue = PixVal(v);}
//    static PixVal get0value()          { return zeroValue;}
//};
//
//template <class Pix>
//ImgVectorizer0x0valueBase::PixVal ImgVectorizer0x0value<Pix>::zeroValue = PixVal(_getZeroValue((Pix*)0));

//////////////////////////////////////////////////////////////////////////

#define _DBGbumpIFcount    (bumpIFcount(__LINE__),true)

class ImgVectorizer0x : public ImgVectorizerBase<InF<8> > {
    //--typedef InF<8>                                   Coord;
    //--typedef std::pair<Coord , Coord >                Point2d;
    //--typedef ImgVectorizerFastAllocator1<Point2d>     ListPoint2dAllocator;
    //--typedef std::list<Point2d,ListPoint2dAllocator>  Plin;
    //--typedef PlinSprout<Coord>                        Sprout;
    //--typedef std::vector< Sprout >                    Sprouts;
    //--typedef std::list <Plin>                         Plins;
    //--typedef ImgVectorizerBase<CoordType>             Base;
    //--typedef Sprouts::iterator                        SproutsIterator;
    //--typedef Sprouts::const_iterator                  SproutsConstIterator;
    //--typedef Plins::iterator                          PlinsIterator;
    //--typedef Plins::const_iterator                    PlinsConstIterator;
    
    typedef PlinSprout<Plin>                         HorzRope;
    typedef std::vector< HorzRope >                  HorzRopes;
    
    //    typedef ImgVectorizer0x0valueBase::PixVal        PixVal;
    typedef InF<4>                                   PixVal;
    
    //
    // segments should be DIRECTED segments (white on the LEFT side)
    
    friend std::ostream& operator<< ( std::ostream& s , const ImgVectorizer0x& iv) {
        s << std::endl << "rowNo=" << iv.m_rowNo << " clms=" << iv.m_clms;
        
        for (PlinsConstIterator it=iv.m_plins.begin(); !(it==iv.m_plins.end()); ++it)
        {
            s << std::endl;
            const Plin& plin = *it;
            for (Plin::const_iterator i = plin.begin(); !(i==plin.end()); ++i )
                s << (i==plin.begin() ? "" : ",  ") << std::setw(6) << i->first << std::setw(6) << i->second;
            
            s << ";" << std::endl;
        }
        
        return s;
    }
    
    friend std::ostream& operator<< (std::ostream& s , const Plins&   plins  ) { return printPlins(s,plins);}
    
    friend std::ostream& operator<< (std::ostream& s , const Sprouts& sprouts) { return printSprouts(s,sprouts);}
    
public:
    typedef     ImgVectorizer0x                   Self;
    
    ~ImgVectorizer0x()         { if (m_prev) delete[] (m_prev-1);}
    
    ImgVectorizer0x ()   // c-tor
    : Base ()
    , m_clipLastRow(false)
    , m_prev(0)
    , m_zeroValue(0)
    {
        bumpIFcount(); // init static DBG if-counts
    }
    
    
    template<class Pix>
    ImgVectorizer0x (Pix v)   // c-tor
    : Base ()
    , m_clipLastRow(false)
    , m_prev(0)
    , m_zeroValue(PixVal(v))
    {
        bumpIFcount(); // init static DBG if-counts
    }
    
    
    ImgVectorizer0x(const ImgVectorizer0x& rhs)       // copy c-tor
    : Base(rhs)
    , m_clipLastRow(rhs.m_clipLastRow)
    , m_prev(rhs.m_prev ? ((new PixVal[m_clms+2])+1) : 0)
    , m_zeroValue(rhs.m_zeroValue)
    {
        bumpIFcount(); // init static DBG if-counts
        if (m_prev && rhs.m_prev)
            memcpy(m_prev-1 , rhs.m_prev-1, (m_clms+2)*sizeof(PixVal));
    }
    
    void operator=(const ImgVectorizer0x& rhs) {
        if (this != &rhs) {
            this->~ImgVectorizer0x();
            std::allocator<ImgVectorizer0x>().construct(this,rhs);
        }
    }
    
    Self& setImgWidth(int clms)           { Base::setImgWidth(clms);        return *this;}
    Self& set1stRowNo(int firstRowNo)     { Base::set1stRowNo(firstRowNo);  return *this;}
    
    template<class Pix>
    Self& set0value(Pix v)                { m_zeroValue = PixVal(v); return *this;}
    
    template < class AnyImg , class PolygonContainer >
    void img2curves ( const AnyImg& img, PolygonContainer* usrPgons )
    {
        img2curves(img, (PolygonContainer*)0, usrPgons);
    }
    
    template < class AnyImg , class PolylinContainer , class PolygonContainer >
    void img2curves ( const AnyImg& img
                     , PolylinContainer* usrPlins
                     , PolygonContainer* usrPgons
                     )
    {
        m_clms = getWidth (img);
        //m_rowNo = -1;
        
        typedef typename AnyImg::Pixel   Pixel;
        if (m_zeroValue==PixVal(0))
            m_zeroValue = PixVal(_getZeroValue((Pixel*)0));
        
        if (m_prev) delete[] (m_prev-1);
        m_prev = 0;
        m_plins.clear();
        
        int rows = getHeight(img);
        
        for (int r=0; r!=rows; ++r)
            addRow( img[r] , usrPlins , usrPgons );
        
        Img<typename AnyImg::Pixel> lastRow(1,m_clms);
        lastRow.rowcpy ( 0 , img[rows-1] );
        if (m_ext==1)                               // polygons only ?
            negateRow ( lastRow[0], Pixel(lastRow[0][0]));               // -yes : negate last row
        else
            m_clipLastRow = true;                   // THE ONLY place, where m_clipLastRow is TRUE
        
        addRow( lastRow[0] , usrPlins , usrPgons );
        
        m_clipLastRow = false;          // maybe for the future usage
        
        if (usrPlins)
            flush(usrPlins);
        else
            flush(usrPgons);
    }
    
    template < class PolycrvContainer >
    void flush ( PolycrvContainer* usrPcrvs )
    {
        // poly-curve is polyline
        while (!m_plins.empty()) {
            moveto ( m_plins.front() , usrPcrvs );
            m_plins.erase(m_plins.begin());
        }
        
        assert (m_plins.size()==0);
    }
    
    
    template < class ConstPixPtr , class PolygonContainer >
    void addRow ( ConstPixPtr currRow , PolygonContainer* usrPgons)
    {
        addRow(currRow , (PolygonContainer*)0, usrPgons);
    }
    
    template < class ConstPixPtr , class PolylinContainer , class PolygonContainer >
    void addRow ( ConstPixPtr currRow
                 , PolylinContainer* usrPlins
                 , PolygonContainer* usrPgons
                 )
    {
        add0xRow(*currRow , currRow , usrPlins , usrPgons);
    }
    
    
    static std::vector<std::pair<int,int> > getIFcounts()
    {
        std::vector<std::pair<int,int> >*  vectorPtr;
        bumpIFcount(0,&vectorPtr);
        return *vectorPtr;
    }
    
private:
    //--int                   m_rowNo;
    //--size_t                m_clms;
    //--ListPoint2dAllocator  m_allocator;
    //--Plins                 m_plins;
    //--int                   m_ext;     // extend rows by 0 or 1 pix on both sides
    //--int                   m_clrUnder;// relevant only in case with only 1 run in prv row
    
    bool           m_clipLastRow;
    PixVal*        m_prev;    // given center-pixel values    (expanded by 2)
    PixVal         m_zeroValue;
    
    
    template <class Pix>
    PixVal get0xValue(Pix pix) const {
        return PixVal(pix) - m_zeroValue;
    }
    
    PixVal get0xValue(bool pix) const { return PixVal(pix ? 16 : -16);}
    
    template<class PixPtr,class Pixel>
    void negateRow(PixPtr row , Pixel) const {
        for (int c=0; c!=m_clms; ++c) {
            PixVal zeroXval = get0xValue(Pixel(row[c]));
            if (zeroXval < PixVal(0))
                continue;
            row[c] = convert_to<Pixel>::from(~zeroXval + m_zeroValue);
        }
    }
    
    static void bumpIFcount(int lineNo=-1 , std::vector<std::pair<int,int> >** allcountsPtr = 0) {
        typedef std::pair<int,int> LineNcount;
        static std::vector<LineNcount> ifcounts;
        if (allcountsPtr) {                 // is it mode GET-INTERNAL-IF-COUNTS ?
            *allcountsPtr = &ifcounts;      // -yes
            return;                         // returned
        }
        if (lineNo<0) {
            ifcounts.clear();
            return;
        }
        
        std::vector<LineNcount>::iterator it = std::lower_bound(ifcounts.begin() , ifcounts.end() , LineNcount(lineNo,0) );
        if (it == ifcounts.end() || it->first != lineNo) {  // was line already passed ?
            it = ifcounts.insert(it,LineNcount(lineNo,0));  // -no add it with 0-counter
        }
        it->second += 1;                                    // increment line-counter
    }
    
    template < class PixelRef , class ConstPixPtr , class PolylinContainer , class PolygonContainer >
    void add0xRow  ( PixelRef
                    , ConstPixPtr currRow
                    , PolylinContainer* usrPlins
                    , PolygonContainer* usrPgons
                    )
    {
        typedef typename PixelTypes<PixelRef>::Pixel    Pixel;
        setExt(usrPlins);
        
        if (m_zeroValue==PixVal(0))
            m_zeroValue = PixVal(_getZeroValue((Pixel*)0));
        
        bool empty = (m_prev==0);   // still no rows added
        ++m_rowNo;
        
        if (m_prev==0) {
            // assert ( _DBGbumpIFcount );
            m_prev = new PixVal[m_clms+2]; ++m_prev; // allocate & immediately increment by 1 spot
            fillPreRow(currRow , (Pixel*)0);
        }
        
        if (empty && m_ext==0) {
            // assert ( _DBGbumpIFcount );
            // special case for 1st row : use only prev
            assert (usrPgons->size()==0);
            assert (usrPlins==0 || usrPlins->size()==0);
            addPreRow (); // create VERTICAL sprouts from prev
            return;
        }
        
        add0xCurrRow ( Pixel() , currRow , usrPlins , usrPgons);
    }
    
    static std::pair<bool,Coord> get0x(PixVal va ,PixVal vb) {
        
        if (PixVal::sameSign(va,vb))
            return std::pair<bool,Coord>(false,Coord());
        
        // va and vb have different sign
        PixVal vd = va-vb;
        Coord x(va.asIntBits() , vd.asIntBits());  // i.e x = va/vd
        
        if (x.isInt()) {
            if (x==Coord(0))
                x = Coord::epsilon();
            else
                x = Coord(1) - Coord::epsilon();
        }
        
        return std::pair<bool,Coord>(true,x);
    }
    
    template < class Pixel , class ConstPixPtr , class PolylinContainer , class PolygonContainer >
    void add0xCurrRow ( const Pixel&
                       , ConstPixPtr currRow
                       , PolylinContainer* usrPlins
                       , PolygonContainer* usrPgons
                       )
    {
        // -----------------------------------------------------------------
        // normal case : we have curr row and sprouts from prev polygons
        // -----------------------------------------------------------------
        
        Plins newPlins;     // ropes in horizontal strip
        
        Coord y0 = Coord(m_rowNo) - halfCell();
        Coord y1 = Coord(m_rowNo) + halfCell();
        
        //  ._____________._____________._____________._____________.
        //  |             |             |             |             |
        //  |             |             |             |             |
        //  |             |             |             |             |
        //  |      *      |      *      |      *      |      *      |
        //  |   curr[-1]  |   curr[0]   |   curr[1]   |   curr[2]   |
        //  |             |             |             |             |
        //  |_____________|_____________|_____________|=============|
        //  |          cur0[0]   m0  cur0[1]   m1  cur0[2]   m2    ||
        //  |             |             |             ||           ||
        //  |             |             |             ||           ||
        //  |      *      |      *      |      *      ||     *     ||
        //  |   prev[-1]  |   prev[0]   |   prev[1]   ||  prev[2]  ||
        //  |             |             |             ||           ||
        //  |_____________|_____________|_____________||===========||
        //             prv0[0]       prv0[1]
        
        PixVal pXprv (m_prev[-1]);
        PixVal cXprv (get0xValue(Pixel(currRow[ 0])) );
        if (m_ext==1) {
            if (cXprv==PixVal(0))
                cXprv = PixVal::epsilon();
            cXprv = -cXprv.abs();
        }
        std::pair<bool,Coord> left_Point = get0x(pXprv,cXprv);
        
        if (left_Point.first) {
            // assert ( _DBGbumpIFcount );
            newPlins.push_back(Plin(m_allocator));
            newPlins.back().push_back(Point2d(Coord(0) , y0+left_Point.second) );
        }
        
        PixVal pXcur;
        PixVal cXcur;
        
        std::pair<bool,Coord> rightPoint;
        
        int nneg = 0;    // # of negative pixels in prev
        
        if (pXprv < PixVal(0))
            ++nneg;
        
        for ( int cellNo = 0; cellNo != m_clms+1;
             ++cellNo
             , (left_Point=rightPoint)
             , (pXprv = pXcur)
             , (cXprv = cXcur)
             )
        {
            pXcur = m_prev[cellNo];
            if (cellNo==m_clms) {
                PixVal c1 = cXprv;
                if (m_ext==1) {
                    if (c1==PixVal(0)) {
                        // assert ( _DBGbumpIFcount );
                        c1 = PixVal::epsilon();
                    }
                    cXcur = -c1.abs();
                }else
                    cXcur = c1;
                m_prev[cellNo] = cXcur;
            }else {
                cXcur = PixVal(get0xValue(Pixel(currRow[cellNo])));
            }
            
            m_prev[cellNo-1] = cXprv;
            
            rightPoint = get0x(pXcur,cXcur);
            
            if (pXcur < PixVal(0)) {
                // assert ( _DBGbumpIFcount );
                ++nneg;
            }
            
            std::pair<bool,Coord> lowerPoint = get0x(pXprv,pXcur);
            std::pair<bool,Coord> upperPoint = get0x(cXprv,cXcur);
            
            size_t vertBorderPoints = 0;
            if (lowerPoint.first) {
                // assert ( _DBGbumpIFcount );
                ++vertBorderPoints;
            }
            
            if (upperPoint.first) {
                // assert ( _DBGbumpIFcount );
                ++vertBorderPoints;
            }
            
            size_t horzBorderPoints = 0;
            
            if (rightPoint.first) {
                // assert ( _DBGbumpIFcount );
                ++horzBorderPoints;
            }
            
            if (left_Point.first) {
                // assert ( _DBGbumpIFcount );
                ++horzBorderPoints;
                assert ( newPlins.size() != 0);
            }
            
            size_t borderPoints = vertBorderPoints+horzBorderPoints;
            
            assert ( borderPoints%2 == 0 );
            
            if (borderPoints==0) {
                // assert ( _DBGbumpIFcount );
                continue;
            }
            
            // i.e. some border points
            bool openNewRope = true;
            // start or end horizontal rope or vertical rope
            //
            // *------@---*  *---@-----*  *--@-----* *---------*
            // |       \  |  |  /      |  | /      | |         |
            // @        \ |  | /       @  |/       | @         |
            // |\        \|  |/       /|  @        | |\        |
            // | \        @  @       / |  |        | | \       |
            // |  \       |  |      /  |  |        | |  \      |
            // *---@------*  *-----@---*  *--------* *---@-----*
            //   case a        case b        case c     case d
            //
            // *--------@-*  *---@---*  *--------*  *-------------*
            // |       /  |  |    \  |  |        |  |             |
            // |      /   |  |     \ |  |        @  @---__        |
            // |     /    |  |      \|  |       /|  |     ---__   |
            // |    /     |  |       @  |      / |  |          ---@
            // |   /      |  |       |  |     /  |  |             |
            // *--@-------*  *-------*  *----@---*  *-------------*
            //    case e       case f      case g       case h
            //
            
            if (borderPoints==4) {
                // assert ( _DBGbumpIFcount );
                
                bool useCaseA = pXprv<0;    // i.e. white-connection is preferable
                
                std::pair<bool,Coord>& lowerORupperPoint = useCaseA ? lowerPoint : upperPoint;
                // set x,y
                Coord x = Coord(int(cellNo)-1) + halfCell() + lowerORupperPoint.second;
                Coord y = useCaseA ? y0 : y1;
                lowerORupperPoint.first = false;
                
                left_Point.first = false;
                
                assert ( newPlins.size() != 0);
                
                Plin& lastPlin = newPlins.back();
                Plin::iterator it = (cXprv < PixVal(0)) ? lastPlin.end() : lastPlin.begin();
                
                lastPlin.insert ( it , Point2d( x , y) );
                
            }else {     // i.e 2 border points
                //
                //    *--@-----* *---------*
                //    | /      | |         |
                //    |/       | @         |
                //    @        | |\        |
                //    |        | | \       |
                //    |        | |  \      |
                //    *--------* *---@-----*
                //        case c     case d
                //
                // *--------@-*  *---@---*  *--------*  *-------------*
                // |       /  |  |    \  |  |        |  |             |
                // |      /   |  |     \ |  |        @  @---__        |
                // |     /    |  |      \|  |       /|  |     ---__   |
                // |    /     |  |       @  |      / |  |          ---@
                // |   /      |  |       |  |     /  |  |             |
                // *--@-------*  *-------*  *----@---*  *-------------*
                //    case e       case f      case g       case h
                //
                if (left_Point.first) {
                    // assert ( _DBGbumpIFcount );
                    //
                    //    *--@-----* *---------*  *-------------*
                    //    | /      | |         |  |             |
                    //    |/       | @         |  @---__        |
                    //    @        | |\        |  |     ---__   |
                    //    |        | | \       |  |          ---@
                    //    |        | |  \      |  |             |
                    //    *--------* *---@-----*  *-------------*
                    //        case c     case d       case h
                    //
                    openNewRope = false;
                    // assert ( _DBGbumpIFcount );
                    // add to the last line
                    assert ( newPlins.size() != 0);
                    Plin& lastPlin = newPlins.back();
                    Plin::iterator it = (cXprv < PixVal(0)) ? lastPlin.end() : lastPlin.begin();
                    
                    Point2d point;
                    if (lowerPoint.first) {          // case d
                        // assert ( _DBGbumpIFcount );
                        point = Point2d(Coord(int(cellNo)-1) + halfCell() + lowerPoint.second , y0);
                    }else {
                        // assert ( _DBGbumpIFcount );
                        if (upperPoint.first) {       // case c
                            // assert ( _DBGbumpIFcount );
                            point = Point2d(Coord(int(cellNo)-1) + halfCell() + upperPoint.second , y1);
                        }else {                       // case h
                            // assert ( _DBGbumpIFcount );
                            Coord xend = Coord(int(cellNo)) + halfCell();
                            if (cellNo==this->m_clms) {
                                // assert ( _DBGbumpIFcount );
                                xend = Coord(int(cellNo));
                            }
                            point = Point2d ( xend , y0 + rightPoint.second );
                        }
                    }
                    lastPlin.insert ( it , point );
                }
            }
            
            if (openNewRope) {
                // assert ( _DBGbumpIFcount );
                //
                // *------@---*  *---@-----*
                // |       \  |  |  /      |
                // @        \ |  | /       @
                // |\        \|  |/       /|
                // | \        @  @       / |
                // |  \       |  |      /  |
                // *---@------*  *-----@---*
                //   case a        case b
                //
                // *--------@-*  *---@---*  *--------*
                // |       /  |  |    \  |  |        |
                // |      /   |  |     \ |  |        @
                // |     /    |  |      \|  |       /|
                // |    /     |  |       @  |      / |
                // |   /      |  |       |  |     /  |
                // *--@-------*  *-------*  *----@---*
                //    case e       case f      case g
                //
                // case a,b,e,f,g
                Point2d pointBeg;
                Point2d pointEnd;
                bool doswap = false;
                if (lowerPoint.first && upperPoint.first) {        // case e
                    // assert ( _DBGbumpIFcount );
                    pointBeg = Point2d ( Coord(int(cellNo)-1) + halfCell() + lowerPoint.second , y0);
                    Coord yend = m_clipLastRow ? Coord(m_rowNo) : y1;
                    pointEnd = Point2d ( Coord(int(cellNo)-1) + halfCell() + upperPoint.second , yend);
                    if (pXcur < PixVal(0)) {
                        // assert ( _DBGbumpIFcount );
                        doswap = true;  // make WHITE to be on the RIGHT side
                    }
                }else {                                            // case a,b,f,g
                    assert (rightPoint.first);
                    pointBeg = Point2d ( Coord(int(cellNo)) + halfCell() , y0 + rightPoint.second );
                    if (upperPoint.first) {                        // case a,f
                        // assert ( _DBGbumpIFcount );
                        pointEnd = Point2d(Coord(int(cellNo)-1) + halfCell() + upperPoint.second , y1);
                        if (cXcur < PixVal(0)) {
                            // assert ( _DBGbumpIFcount );
                            doswap = true;  // make WHITE to be on the RIGHT side
                        }
                    }else {
                        // assert ( _DBGbumpIFcount );
                        assert (lowerPoint.first);                 // case b,g
                        pointEnd = Point2d(Coord(int(cellNo)-1) + halfCell() + lowerPoint.second , y0);
                        if (pXprv < PixVal(0)) {
                            // assert ( _DBGbumpIFcount );
                            doswap = true;  // make WHITE to be on the RIGHT side
                        }
                    }
                }
                
                if (doswap) {
                    // assert ( _DBGbumpIFcount );
                    std::swap(pointBeg , pointEnd);
                }
                
                Plin newPlin(m_allocator);
                newPlin.push_back(pointBeg);
                newPlin.push_back(pointEnd);
                
                newPlins.push_back(newPlin);
            }
        }
        
        m_clrUnder    = (nneg==0 ? 1 : (nneg==m_clms+2 ? -1 : 0));
        
        // create sorted array of polylin sprouts (from plins),
        // moving finished polylines into usrPlins
        
        Sprouts sprouts;
        
        if (usrPlins)
            createSortedSprouts(sprouts , usrPlins);
        else
            createSortedSprouts(sprouts , usrPgons);
        
        //clog << "--- ROW --- " << m_rowNo << " after create sprouts --- " << std::endl << " -- sprouts --" << std::endl << sprouts << std::endl;
        
        // newPlins created:
        // now put them into 'connected-down plins' and 'up-oriented-plins;
        Plins connectedDown1plins;
        Plins connectedDown2plins;
        Plins upOrientedPlins;
        
        if (sprouts.size()==0) {
            // assert ( _DBGbumpIFcount );
            upOrientedPlins = newPlins;
        }else {
            // assert ( _DBGbumpIFcount );
            for (Plins::const_iterator it=newPlins.begin(); it != newPlins.end(); ++it) {
                const Plin& plin = *it;
                Coord yf = plin.front().second;
                Coord yb = plin.back ().second;
                if (yf==y0 ||  yb==y0) {
                    // assert ( _DBGbumpIFcount );
                    // in new polyline one or more edges lays on y0
                    if (yb==yf) {
                        // assert ( _DBGbumpIFcount );
                        // in new polyline both edges lay on y0
                        connectedDown2plins.push_back(plin);
                    }else {
                        // assert ( _DBGbumpIFcount );
                        // in new polyline only one edge lays on y0
                        connectedDown1plins.push_back(plin);
                    }
                }else {
                    // assert ( _DBGbumpIFcount );
                    // in new polyline both edges lays above y0
                    upOrientedPlins.push_back(plin);
                }
            }
        }
        // now 1. connect connectedDownPlins with just created sprouts
        //     2. copy upOrientedPlins into internal plins
        
        //clog << "--- ROW --- " << m_rowNo << " before merge --- " << std::endl << " -- sprouts --" << std::endl << sprouts << std::endl;
        //clog << "--- ROW --- " << m_rowNo << " before merge --- " << std::endl << " -- connectedDown2plins --" << std::endl << connectedDown2plins << std::endl;
        
        mergeSprouts( sprouts , connectedDown2plins , usrPgons);
        
        //clog << "--- ROW --- " << m_rowNo << " before growSprouts --- " << std::endl << " -- sprouts --" << std::endl << sprouts << std::endl;
        //clog << "--- ROW --- " << m_rowNo << " before growSprouts --- " << std::endl << " -- connectedDown1plins --" << std::endl << connectedDown1plins << std::endl;
        
        growSprouts ( sprouts , connectedDown1plins );
        
        //clog << "--- ROW --- " << m_rowNo << " after growSprouts --- " << std::endl << " -- sprouts --" << std::endl << sprouts << std::endl;
        //clog << "--- ROW --- " << m_rowNo << " after growSprouts --- " << std::endl << " -- connectedDown1plins --" << std::endl << connectedDown1plins << std::endl;
        
        addNewPlins ( upOrientedPlins );
        //        killEmptyPlins();
        
    }
    
    template < class ConstPixPtr , class Pixel>
    void fillPreRow ( ConstPixPtr currRow , Pixel*)
    {
        assert ( m_plins. size()==0);
        
        int nneg = 0;
        
        for (int c=0; c!=m_clms; ++c) {
            PixVal curVal(get0xValue(Pixel(currRow[c])));
            if (m_ext) {   // i.e. no polylines
                if (curVal==PixVal(0)) {
                    // assert ( _DBGbumpIFcount );
                    curVal = PixVal::epsilon();
                }
                m_prev[c] = -curVal.abs();
                ++nneg;
            }else {
                m_prev[c] =  curVal;
                if (curVal < PixVal(0)) {
                    // assert ( _DBGbumpIFcount );
                    ++nneg;
                }
            }
        }
        if (m_ext) {
            m_prev[   -1 ] = -m_prev[      0 ].abs();
            m_prev[m_clms] = -m_prev[m_clms-1].abs();
            nneg += 2;
        }else {
            m_prev[   -1 ] =  m_prev[      0 ];
            m_prev[m_clms] =  m_prev[m_clms-1];
            if (m_prev[ -1 ] < PixVal(0)) {
                ++nneg;
            }
            
            if (m_prev[m_clms] < PixVal(0)) {
                ++nneg;
            }
        }
        
        m_clrUnder = (nneg==0 ? 1 : (nneg==(m_clms+2) ? -1 : 0));
    }
    
    void addPreRow ()  // create VERTICAL sprouts from prev
    {
        assert ( m_plins. size()==0);
        
        PixVal c0 = m_prev[-1];
        
        Coord  x0 = -halfCell();
        Coord  y0 = Coord(m_rowNo) - halfCell();
        Coord  y1 = Coord(m_rowNo) + halfCell();
        if (y0 < Coord(0)) {
            // assert ( _DBGbumpIFcount );
            y0 = Coord(0);
        }
        
        for (int clm=0; clm != m_clms; ++clm) {
            // create STICK on the rght side of current run
            Coord x1 = x0+Coord(1);
            PixVal c1 = m_prev[clm];
            std::pair<bool,Coord> xpoint = get0x(c0,c1);
            if (xpoint.first) {
                // assert ( _DBGbumpIFcount );
                Coord x = x0+xpoint.second;
                assert ( x >= Coord(0) );
                Point2d begVert ( x , y0);
                Point2d endVert ( x , y1);
                if (c1 < PixVal(0)) {
                    // assert ( _DBGbumpIFcount );
                    std::swap(begVert,endVert);
                }
                
                Plin vert(m_allocator);
                
                vert.insert(vert.end() , begVert);
                vert.insert(vert.end() , endVert);
                
                m_plins.insert(m_plins.end() , vert );
            }
            x0 = x1;
            c0 = c1;
        }
    }
    
    template < class PolygonContainer >
    void mergeSprouts ( Sprouts& sprouts  // sorted by x
                       , Plins& connectedDown2plins
                       , PolygonContainer* usrPgons)
    {
        Coord xplin(0);
        Coord y0 = Coord(m_rowNo) - halfCell();
        for (Plins::iterator it = connectedDown2plins.begin(); it != connectedDown2plins.end(); ++it)
        {
            if (it->empty()) {
                // assert ( _DBGbumpIFcount );
                continue;
            }
            
            Plin& plin = *it;
            
            Point2d left = plin.front();
            Point2d rght = plin.back ();
            
            bool forwardRope = (left.first < rght.first);
            if (!forwardRope) {
                // assert ( _DBGbumpIFcount );
                std::swap(left,rght);
            }
            
            
            assert (left.second==y0 && rght.second==y0);
            
            assert (xplin <= left.first);
            
            xplin = left.first;
            
            SproutsIterator itSprouts = sprouts.begin();
            for (;;++itSprouts) {
                if (!(itSprouts != sprouts.end()))return;
                if (itSprouts->empty()) {
                    // assert ( _DBGbumpIFcount );
                    continue;
                }
                
                if (itSprouts->thisVertexX() != xplin) {
                    // assert ( _DBGbumpIFcount );
                    continue;
                }
                // now ck also y-coord: maybe it is just down-ROPE
                if (itSprouts->thisVertexY() != y0) {
                    // assert ( _DBGbumpIFcount );
                    continue;
                }
                
                break;
            }
            //               __   or        ___       or   ___  or  ___         or  _________         |
            //              /  \           /   \          /   \    /   \           /         \        |
            //  *    *     @    *    *    @     *    *   @     *  @     *     *   @     *     *       |
            //  |    \____/    /     |   /       \__/     \___/    \_________/     \___/              |
            //   \____________/       \_/
            //       case a                case b         case c     case d          case e
            
            Sprout horzRope(&plin , forwardRope );
            // kill start & end points in current plin
            plin.pop_front();
            plin.pop_back ();
            
            xplin = rght.first;     // other edge
            if (itSprouts->scndVertexX() == xplin) {
                // assert ( _DBGbumpIFcount );
                //    ___  			|
                //   /   \ 			|
                //  @     *			|
                //   \___/ 			|
                //   case c
                //
                horzRope.moveFrom(*itSprouts);
                // move current closed plin into usrPgons
                moveto(plin,usrPgons);
                assert(plin.empty());
            }else {
                // assert ( _DBGbumpIFcount );                                              |
                //               __   or        ___       or  ___         or  _________     |
                //              /  \           /   \         /   \           /         \    |
                //  *    *     @    *    *    @     *    *  @     *     *   @     *--   *   |
                //  |    \____/    /     |   /       \__/    \_________/     \___/          |
                //   \____________/       \_/
                //       case a                case b          case d          case e
                
                // now lets find 2nd sprout
                SproutsIterator jtSprouts = sprouts.begin();
                for (;;++jtSprouts) {
                    if (!(jtSprouts != sprouts.end()))return;
                    if (jtSprouts->empty()) {
                        // assert ( _DBGbumpIFcount );
                        continue;
                    }
                    
                    if (jtSprouts->thisVertexX() != xplin) {
                        // assert ( _DBGbumpIFcount );
                        continue;
                    }
                    
                    // now ck also y-coord: maybe it is just down-ROPE
                    if (jtSprouts->thisVertexY() != y0) {
                        // assert ( _DBGbumpIFcount );
                        continue;
                    }
                    
                    // assert ( _DBGbumpIFcount );
                    break;
                }
                // now lets connect 3 ropes: itSprout,plin,jtSprout
                itSprouts->moveFrom(horzRope);
                jtSprouts->moveFrom(*itSprouts);
            }
            
        }
    }
    
    void growSprouts ( Sprouts& sprouts  // sorted by x
                      , Plins& connectedDown1plins)
    {
        Coord xplin(0);
        
        for (Plins::iterator it = connectedDown1plins.begin(); it != connectedDown1plins.end(); ++it)
        {
            assert ( ! it->empty());
            
            Plin& plin = *it;
            
            Point2d lower = plin.front();
            Point2d upper = plin.back ();
            
            bool toupRope = (lower.second < upper.second);
            if (!toupRope) {
                // assert ( _DBGbumpIFcount );
                std::swap ( lower , upper );
            }
            
            Coord begRopeY = lower.second;        
            
            Sprout horzRope(&plin , toupRope);
            
            assert (xplin <= lower.first);
            xplin = lower.first;
            
            assert ( horzRope.thisVertexX() == xplin);
            
            SproutsIterator itSprouts = sprouts.begin();
            for (;;++itSprouts) {
                if (!(itSprouts != sprouts.end()))return;
                if (itSprouts->empty()) {
                    // assert ( _DBGbumpIFcount );
                    continue;
                }
                
                if (itSprouts->thisVertexX() != xplin) {
                    // assert ( _DBGbumpIFcount );
                    continue;
                }
                
                if (itSprouts->thisVertexY() != begRopeY) {
                    // assert ( _DBGbumpIFcount );
                    continue;
                }
                
                // assert ( _DBGbumpIFcount );
                break;
            }
            
            if (toupRope) {
                // assert ( _DBGbumpIFcount );
                plin.pop_front();   // kill 1-st edge
            }else {
                // assert ( _DBGbumpIFcount );
                plin.pop_back ();   // kill last edge
            }
            // move CURRENT plin into CURRENT sprout
            itSprouts->moveFrom(horzRope);
        }        
    }
};

#endif
