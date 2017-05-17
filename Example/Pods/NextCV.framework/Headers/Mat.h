/*M///////////////////////////////////////////////////////////////////////////////////////
 //
 //  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
 //
 //  By downloading, copying, installing or using the software you agree to this license.
 //  If you do not agree to this license, do not download, install,
 //  copy or use the software.
 //
 //
 //                           License Agreement
 //                For Open Source Computer Vision Library
 //
 // Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
 // Copyright (C) 2009-2010, Willow Garage Inc., all rights reserved.
 // Third party copyrights are property of their respective owners.
 //
 // Redistribution and use in source and binary forms, with or without modification,
 // are permitted provided that the following conditions are met:
 //
 //   * Redistribution's of source code must retain the above copyright notice,
 //     this list of conditions and the following disclaimer.
 //
 //   * Redistribution's in binary form must reproduce the above copyright notice,
 //     this list of conditions and the following disclaimer in the documentation
 //     and/or other materials provided with the distribution.
 //
 //   * The name of the copyright holders may not be used to endorse or promote products
 //     derived from this software without specific prior written permission.
 //
 // This software is provided by the copyright holders and contributors "as is" and
 // any express or implied warranties, including, but not limited to, the implied
 // warranties of merchantability and fitness for a particular purpose are disclaimed.
 // In no event shall the Intel Corporation or contributors be liable for any direct,
 // indirect, incidental, special, exemplary, or consequential damages
 // (including, but not limited to, procurement of substitute goods or services;
 // loss of use, data, or profits; or business interruption) however caused
 // and on any theory of liability, whether in contract, strict liability,
 // or tort (including negligence or otherwise) arising in any way out of
 // the use of this software, even if advised of the possibility of such damage.
 //
 //M*/

//
//  Mat.h
//  NextCV
//

#ifndef Mat_hpp
#define Mat_hpp

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include "NCVCommon.h"

namespace ncv {
    class Point2i
    {
    public:
        
        // various constructors
        Point2i();
        Point2i(int _x, int _y);
        Point2i(const Point2i& pt);
        
        Point2i& operator = (const Point2i& pt);
        
        int x, y; //< the point coordinates
    };
    
    class Point2f
    {
    public:
        
        // various constructors
        Point2f();
        Point2f(float _x, float _y);
        Point2f(const Point2f& pt);
        
        Point2f& operator = (const Point2f& pt);
        
        float x, y; //< the point coordinates
    };

    class Size
    {
    public:
        
        //! various constructors
        Size();
        Size(int _width, int _height);
        Size(const Size& sz);
        
        Size& operator = (const Size& sz);
        //! the area (width*height)
        int area() const;
        
        int width, height; // the width and the height
    };
    
    class Rect
    {
    public:
        typedef int value_type;
        
        //! various constructors
        Rect();
        Rect(int _x, int _y, int _width, int _height);
        Rect(const ncv::Rect& r);
        
        ncv::Rect& operator = ( const ncv::Rect& r );
        
        //! size (width, height) of the rectangle
        Size size() const;
        //! area (width*height) of the rectangle
        int area() const;
        
        int x, y, width, height; //< the top-left corner, as well as width and height of the rectangle
    };
    
    class Range
    {
    public:
        Range();
        Range(int _start, int _end);
        int size() const;
        bool empty() const;
        static Range all();
        
        int start, end;
    };

    class RNG
    {
    public:
        enum { UNIFORM=0, NORMAL=1 };
        
        RNG();
        RNG(uint64 state);
        //! updates the state and returns the next 32-bit unsigned integer random number
        unsigned next();
        
        operator uchar();
        operator schar();
        operator ushort();
        operator short();
        operator unsigned();
        //! returns a random integer sampled uniformly from [0, N).
        unsigned operator ()(unsigned N);
        unsigned operator ()();
        operator int();
        operator float();
        operator double();
        //! returns uniformly distributed integer random number from [a,b) range
        int uniform(int a, int b);
        //! returns uniformly distributed floating-point random number from [a,b) range
        float uniform(float a, float b);
        //! returns uniformly distributed double-precision floating-point random number from [a,b) range
        double uniform(double a, double b);
        //void fill( Mat & mat, int distType, Mat & a, Mat & b, bool saturateRange=false );
        //! returns Gaussian random variate with mean zero.
        //double gaussian(double sigma);
        
        uint64 state;
    };
    
    // Multiply-with-Carry RNG
    inline RNG::RNG() { state = 0xffffffff; }
    inline RNG::RNG(uint64 _state) { state = _state ? _state : 0xffffffff; }
    inline unsigned RNG::next()
    {
        state = (uint64)(unsigned)state*CV_RNG_COEFF + (unsigned)(state >> 32);
        return (unsigned)state;
    }
    
    inline RNG::operator uchar() { return (uchar)next(); }
    inline RNG::operator schar() { return (schar)next(); }
    inline RNG::operator ushort() { return (ushort)next(); }
    inline RNG::operator short() { return (short)next(); }
    inline RNG::operator unsigned() { return next(); }
    inline unsigned RNG::operator ()(unsigned N) {return (unsigned)uniform(0,N);}
    inline unsigned RNG::operator ()() {return next();}
    inline RNG::operator int() { return (int)next(); }
    // * (2^32-1)^-1
    inline RNG::operator float() { return next()*2.3283064365386962890625e-10f; }
    inline RNG::operator double()
    {
        unsigned t = next();
        return (((uint64)t << 32) | next())*5.4210108624275221700372640043497e-20;
    }
    inline int RNG::uniform(int a, int b) { return a == b ? a : (int)(next()%(b - a) + a); }
    inline float RNG::uniform(float a, float b) { return ((float)*this)*(b - a) + a; }
    inline double RNG::uniform(double a, double b) { return ((double)*this)*(b - a) + a; }
    
    template<typename _Tp> _Tp* allocate(size_t n)
    {
        return new _Tp[n];
    }
    
    template<typename _Tp> void deallocate(_Tp* ptr, size_t)
    {
        delete[] ptr;
    }

    template<typename _Tp, size_t fixed_size=4096/sizeof(_Tp)+8> class AutoBuffer
    {
    public:
        typedef _Tp value_type;
        enum { buffer_padding = (int)((16 + sizeof(_Tp) - 1)/sizeof(_Tp)) };
        
        //! the default contructor
        AutoBuffer();
        //! constructor taking the real buffer size
        AutoBuffer(size_t _size);
        //! destructor. calls deallocate()
        ~AutoBuffer();
        
        //! allocates the new buffer of size _size. if the _size is small enough, stack-allocated buffer is used
        void allocate(size_t _size);
        //! deallocates the buffer if it was dynamically allocated
        void deallocate();
        //! returns pointer to the real buffer, stack-allocated or head-allocated
        operator _Tp* ();
        //! returns read-only pointer to the real buffer, stack-allocated or head-allocated
        operator const _Tp* () const;
        
    protected:
        //! pointer to the real buffer, can point to buf if the buffer is small enough
        _Tp* ptr;
        //! size of the real buffer
        size_t size;
        //! pre-allocated buffer
        _Tp buf[fixed_size+buffer_padding];
    };

    /////////////////////////////// AutoBuffer ////////////////////////////////////////
    
    template<typename _Tp, size_t fixed_size> inline AutoBuffer<_Tp, fixed_size>::AutoBuffer()
    {
        ptr = buf;
        size = fixed_size;
    }
    
    template<typename _Tp, size_t fixed_size> inline AutoBuffer<_Tp, fixed_size>::AutoBuffer(size_t _size)
    {
        ptr = buf;
        size = fixed_size;
        allocate(_size);
    }
    
    template<typename _Tp, size_t fixed_size> inline AutoBuffer<_Tp, fixed_size>::~AutoBuffer()
    { deallocate(); }
    
    template<typename _Tp, size_t fixed_size> inline void AutoBuffer<_Tp, fixed_size>::allocate(size_t _size)
    {
        if(_size <= size)
            return;
        deallocate();
        if(_size > fixed_size)
        {
            ptr = ncv::allocate<_Tp>(_size);
            size = _size;
        }
    }
    
    template<typename _Tp, size_t fixed_size> inline void AutoBuffer<_Tp, fixed_size>::deallocate()
    {
        if( ptr != buf )
        {
            ncv::deallocate<_Tp>(ptr, size);
            ptr = buf;
            size = fixed_size;
        }
    }
    
    template<typename _Tp, size_t fixed_size> inline AutoBuffer<_Tp, fixed_size>::operator _Tp* ()
    { return ptr; }
    
    template<typename _Tp, size_t fixed_size> inline AutoBuffer<_Tp, fixed_size>::operator const _Tp* () const
    { return ptr; }

    class SimilarRects
    {
    public:
        SimilarRects(double _eps) : eps(_eps) {}
        inline bool operator()(const ncv::Rect& r1, const ncv::Rect& r2) const
        {
            double delta = eps*(std::min(r1.width, r2.width) + std::min(r1.height, r2.height))*0.5;
            return abs(r1.x - r2.x) <= delta &&
            abs(r1.y - r2.y) <= delta &&
            abs(r1.x + r1.width - r2.x - r2.width) <= delta &&
            abs(r1.y + r1.height - r2.y - r2.height) <= delta;
        }
        double eps;
    };

    template<typename _Tp, int cn> class Vec
    {
    public:
        typedef _Tp value_type;
        
        //! default constructor
        Vec();
        
        /*! element access */
        const _Tp& operator [](int i) const;
        _Tp& operator[](int i);
    
        _Tp val[cn]; //< matrix elements
    };

    
    /* ---- Vec ---- */
    template<typename _Tp, int cn> Vec<_Tp, cn>::Vec()
    {}
    
    template<typename _Tp, int cn> const _Tp& Vec<_Tp, cn>::operator [](int i) const
    {
        return this->val[i];
    }
    
    template<typename _Tp, int cn> _Tp& Vec<_Tp, cn>::operator [](int i)
    {
        return this->val[i];
    }

    typedef Vec<uchar, 2> Vec2b;
    typedef Vec<uchar, 3> Vec3b;
    typedef Vec<uchar, 4> Vec4b;
    typedef Vec<int, 2> Vec2i;
    typedef Vec<float, 2> Vec2f;
    typedef Vec<int, 3> Vec3i;
    typedef Vec<int, 4> Vec4i;
    typedef Vec<double, 4> Vec4d;
    
    /*!
     The template scalar class.
     
     This is partially specialized cv::Vec class with the number of elements = 4, i.e. a short vector of four elements.
     Normally, cv::Scalar ~ cv::Scalar_<double> is used.
     */
    template<typename _Tp> class Scalar_ : public Vec<_Tp, 4>
    {
    public:
        //! various constructors
        Scalar_();
        Scalar_(_Tp v0, _Tp v1, _Tp v2=0, _Tp v3=0);
        //Scalar_(const CvScalar& s);
        Scalar_(_Tp v0);
        
        //! returns a scalar with all elements set to v0
        static Scalar_<_Tp> all(_Tp v0);
        //! conversion to the old-style CvScalar
        //operator CvScalar() const;
        
        //! conversion to another data type
        //template<typename T2> operator Scalar_<T2>() const;
        
        //! per-element product
        //Scalar_<_Tp> mul(const Scalar_<_Tp>& t, double scale=1 ) const;
        
        // returns (v0, -v1, -v2, -v3)
        //Scalar_<_Tp> conj() const;
        
        // returns true iff v1 == v2 == v3 == 0
        bool isReal() const;
    };
    typedef Scalar_<double> Scalar;
    //////////////////////////////// Scalar_ ///////////////////////////////
    template<typename _Tp> inline Scalar_<_Tp>::Scalar_()
    { this->val[0] = this->val[1] = this->val[2] = this->val[3] = 0; }
    
    template<typename _Tp> inline Scalar_<_Tp>::Scalar_(_Tp v0, _Tp v1, _Tp v2, _Tp v3)
    { this->val[0] = v0; this->val[1] = v1; this->val[2] = v2; this->val[3] = v3; }
    
    template<typename _Tp> inline Scalar_<_Tp>::Scalar_(_Tp v0)
    { this->val[0] = v0; this->val[1] = this->val[2] = this->val[3] = 0; }
    
    template<typename _Tp> inline Scalar_<_Tp> Scalar_<_Tp>::all(_Tp v0)
    { return Scalar_<_Tp>(v0, v0, v0, v0); }
    
    template<typename _Tp> inline bool Scalar_<_Tp>::isReal() const
    {
        return this->val[1] == 0 && this->val[2] == 0 && this->val[3] == 0;
    }
    
    class MatAllocator
    {
    public:
        MatAllocator() {}
        virtual ~MatAllocator() {}
        virtual void allocate(int dims, const int* sizes, int type, int*& refcount,
                              uchar*& datastart, uchar*& data, size_t* step) = 0;
        virtual void deallocate(int* refcount, uchar* datastart, uchar* data) = 0;
    };
    
    class MatExpr;
    class Mat
    {
    public:
        //! default constructor
        Mat();
        //! constructs 2D matrix of the specified size and type
        // (_type is CV_8UC1, CV_64FC3, CV_32SC(12) etc.)
        Mat(int rows, int cols, int type);
        Mat(Size size, int type);
        
        //! copy constructor
        Mat(const Mat& m);
        
        Mat(const Mat& m, const ncv::Rect& roi);
        
        Mat(int rows, int cols, int type, void* data, size_t step=AUTO_STEP);
        
        Mat(const Mat& m, const Range& rowRange, const Range& colRange=Range::all());
        Mat(const Mat& m, const Range* ranges);

        //! destructor - calls release()
        ~Mat();
        //! assignment operators
        Mat& operator = (const Mat& m);
        Mat& operator = (const Scalar& s);
        Mat& operator = (const MatExpr& e);

        Mat& setTo(uchar value);
        
        MatExpr t() const;
        MatExpr inv(int method=DECOMP_LU) const;
        Mat mul(const Mat & m, double scale=1) const;
        
        //! returns a new matrix header for the specified row
        Mat row(int y) const;
        //! returns a new matrix header for the specified column
        Mat col(int x) const;
        
        //! ... for the specified row span
        Mat rowRange(int startrow, int endrow) const;
        Mat rowRange(const Range& r) const;
        //! ... for the specified column span
        Mat colRange(int startcol, int endcol) const;
        Mat colRange(const Range& r) const;
    
        //! returns deep copy of the matrix, i.e. the data is copied
        Mat clone() const;
        
        //! copies the matrix content to "m".
        // It calls m.create(this->size(), this->type()).
        void copyTo( Mat &m ) const;
        
        //only support scale/shift for the SAME TYPE of matrix; and only support INT to FLOAT convert.
        void convertTo( Mat & dst, int _type, double alpha=1, double beta=0 ) const;
        
        //! allocates new matrix data unless the matrix already has specified size and type.
        // previous data is unreferenced if needed.
        void create(int rows, int cols, int type);
        void create(Size size, int type);
        void create(int d, const int* _sizes, int _type);
        
        //! increases the reference counter; use with care to avoid memleaks
        void addref();
        //! decreases reference counter;
        // deallocates the data when reference counter reaches 0.
        void release();
        
        //! deallocates the matrix data
        void deallocate();
        
        //! locates matrix header within a parent matrix. See below
        void locateROI( Size& wholeSize, Point2i& ofs ) const;
        
        //! moves/resizes the current matrix ROI inside the parent matrix.
        Mat& adjustROI( int dtop, int dbottom, int dleft, int dright );

        //! extracts a rectangular sub-matrix
        Mat operator()( const ncv::Rect& roi ) const;
        Mat operator()( const ncv::Range* ranges ) const;
        
        bool isContinuous() const;
        bool isSubmatrix() const;
        
        static MatExpr eye(int rows, int cols, int type);
        static MatExpr eye(Size size, int type);
        
        //! returns element size in bytes,
        // similar to CV_ELEM_SIZE(cvmat->type)
        size_t elemSize() const;
        //! returns the size of element channel in bytes.
        size_t elemSize1() const;
        //! returns element type, similar to CV_MAT_TYPE(cvmat->type)
        int type() const;
        //! returns element type, similar to CV_MAT_DEPTH(cvmat->type)
        int depth() const;
        //! returns element type, similar to CV_MAT_CN(cvmat->type)
        int channels() const;
        //! returns true if matrix data is NULL
        bool empty() const;
        //! returns the total number of matrix elements
        size_t total() const;
        
        size_t step1(int i=0) const;

        //! the same as above, with the pointer dereferencing
        template<typename _Tp> _Tp& at(int i0=0);
        template<typename _Tp> const _Tp& at(int i0=0) const;
        
        template<typename _Tp> _Tp& at(int i0, int i1);
        template<typename _Tp> const _Tp& at(int i0, int i1) const;
        
        uchar* ptr(int i0=0);
        const uchar* ptr(int i0=0) const;
        
        template<typename _Tp> _Tp* ptr(int y=0);
        template<typename _Tp> const _Tp* ptr(int y=0) const;
        
        enum { MAGIC_VAL=0x42FF0000, AUTO_STEP=0, CONTINUOUS_FLAG=CV_MAT_CONT_FLAG, SUBMATRIX_FLAG=CV_SUBMAT_FLAG };
        
        /*! includes several bit-fields:
         - the magic signature
         - continuity flag
         - depth
         - number of channels
         */
        int flags;
        //! the matrix dimensionality, >= 2
        int dims;
        //! the number of rows and columns or (-1, -1) when the matrix has more than 2 dimensions
        int rows, cols;
        //! pointer to the data
        uchar* data;
        
        //! pointer to the reference counter;
        // when matrix points to user-allocated data, the pointer is NULL
        int* refcount;
        
        //! helper fields used in locateROI and adjustROI
        uchar* datastart;
        uchar* dataend;
        uchar* datalimit;
        
        //! custom allocator
        MatAllocator* allocator;
        
        struct MSize
        {
            MSize(int* _p);
            Size operator()() const;
            const int& operator[](int i) const;
            int& operator[](int i);
            operator const int*() const;
            bool operator == (const MSize& sz) const;
            bool operator != (const MSize& sz) const;
            
            int* p;
        };
        
        struct MStep
        {
            MStep();
            MStep(size_t s);
            const size_t& operator[](int i) const;
            size_t& operator[](int i);
            operator size_t() const;
            MStep& operator = (size_t s);
            
            size_t* p;
            size_t buf[2];
        protected:
            MStep& operator = (const MStep&);
        };
        
        MSize size;
        MStep step;
        
    protected:
        void initEmpty();
        //! internal use function; properly re-allocates _size, _step arrays
        void copySize(const Mat& m);
        bool fixedType() const;
        Size getContinuousSize( const Mat& m1, const Mat& m2, int widthScale=1 ) const;
    };
    
    class MatExpr;
    class MatOp
    {
    public:
        MatOp() {};
        virtual ~MatOp() {};

        virtual void assign(const MatExpr& expr, Mat& m, int type=-1) const = 0;
    };
    
    class MatExpr
    {
    public:
        MatExpr() : op(0), flags(0), a(Mat()), b(Mat()), c(Mat()), alpha(0), beta(0), s(Scalar()) {}
        MatExpr(const MatOp* _op, int _flags, const Mat& _a=Mat(), const Mat& _b=Mat(),
                const Mat& _c=Mat(), double _alpha=1, double _beta=1, const Scalar& _s=Scalar())
        : op(_op), flags(_flags), a(_a), b(_b), c(_c), alpha(_alpha), beta(_beta), s(_s){}
        explicit MatExpr(const Mat& m);
        operator Mat() const
        {
            Mat m;
            op->assign(*this, m);
            return m;
        }
        
        MatExpr row(int y) const;
        MatExpr col(int x) const;
        MatExpr diag(int d=0) const;
        MatExpr operator()( const Range& rowRange, const Range& colRange ) const;
        MatExpr operator()( const Rect& roi ) const;
        
        Mat cross(const Mat& m) const;
        double dot(const Mat& m) const;
        
        MatExpr t() const;
        MatExpr inv(int method = DECOMP_LU) const;
        MatExpr mul(const MatExpr& e, double scale=1) const;
        MatExpr mul(const Mat& m, double scale=1) const;
        
        Size size() const;
        int type() const;
        
        const MatOp* op;
        int flags;
        
        Mat a, b, c;
        double alpha, beta;
        ncv::Scalar s;
    };
    
    template<typename _Tp> inline _Tp& Mat::at(int i0)
    {
        if( isContinuous() || size.p[0] == 1 )
            return ((_Tp*)data)[i0];
        if( size.p[1] == 1 )
            return *(_Tp*)(data + step.p[0]*i0);
        int i = i0/cols, j = i0 - i*cols;
        return ((_Tp*)(data + step.p[0]*i))[j];
    }
    
    template<typename _Tp> inline const _Tp& Mat::at(int i0) const
    {
        if( isContinuous() || size.p[0] == 1 )
            return ((const _Tp*)data)[i0];
        if( size.p[1] == 1 )
            return *(const _Tp*)(data + step.p[0]*i0);
        int i = i0/cols, j = i0 - i*cols;
        return ((const _Tp*)(data + step.p[0]*i))[j];
    }
    
    template<typename _Tp> inline _Tp& Mat::at(int i0, int i1)
    {
        return ((_Tp*)(data + step.p[0]*i0))[i1];
    }
    
    template<typename _Tp> inline const _Tp& Mat::at(int i0, int i1) const
    {
        return ((const _Tp*)(data + step.p[0]*i0))[i1];
    }

    template<typename _Tp> inline _Tp* Mat::ptr(int y)
    {
        return (_Tp*)(data + step.p[0] * y);
    }
    
    template<typename _Tp> inline const _Tp* Mat::ptr(int y) const
    {
        return (const _Tp*)(data + step.p[0] * y);
    }
    
    class NAryMatIterator
    {
    public:
        //! the default constructor
        NAryMatIterator();
        //! the full constructor taking arbitrary number of n-dim matrices
        NAryMatIterator(const Mat** arrays, uchar** ptrs, int narrays=-1);
        //! the full constructor taking arbitrary number of n-dim matrices
        NAryMatIterator(const Mat** arrays, Mat* planes, int narrays=-1);
        //! the separate iterator initialization method
        void init(const Mat** arrays, Mat* planes, uchar** ptrs, int narrays=-1);
        
        //! proceeds to the next plane of every iterated matrix
        NAryMatIterator& operator ++();
        //! proceeds to the next plane of every iterated matrix (postfix increment operator)
        NAryMatIterator operator ++(int);
        
        //! the iterated arrays
        const Mat** arrays;
        //! the current planes
        Mat* planes;
        //! data pointers
        uchar** ptrs;
        //! the number of arrays
        int narrays;
        //! the number of hyper-planes that the iterator steps through
        size_t nplanes;
        //! the size of each segment (in elements)
        size_t size;
    protected:
        int iterdepth;
        size_t idx;
    };
    
    class SVD
    {
    public:
        enum { MODIFY_A=1, NO_UV=2, FULL_UV=4 };
        //! the default constructor
        SVD();
        //! the constructor that performs SVD
        SVD( const Mat & src, int flags=0 );
        //! the operator that performs SVD. The previously allocated SVD::u, SVD::w are SVD::vt are released.
        SVD& operator ()( const Mat & src, int flags=0 );
        
        //! decomposes matrix and stores the results to user-provided matrices
        static void compute( const Mat & src, Mat & w,
                            Mat & u, Mat & vt, int flags=0 );
        
        //! performs back substitution
        static void backSubst( const Mat & w, const Mat & u,
                              const Mat & vt, const Mat & rhs,
                              Mat & dst );
        
        Mat u, w, vt;
    };
    
    double invert( const Mat & src, Mat & dst, int method=DECOMP_LU );
    void integral(const ncv::Mat & src, ncv::Mat & sum);
    void cvtColor(const ncv::Mat & src, ncv::Mat & dst, const int & type);
    //int resize(const ncv::Mat & src, ncv::Mat & dst, const ncv::Size & dsize, const double & fx_1=0, const double & fy_1=0, const int & interpolation=NCV_INTER_LINEAR);
    void resize(const ncv::Mat & src, ncv::Mat & dst, ncv::Size dsize, double fx_1=0, double fy_1=0, int interpolation=NCV_INTER_LINEAR);
    
    template<typename _Tp, class _EqPredicate> int partition( const std::vector<_Tp>& _vec, std::vector<int>& labels, _EqPredicate predicate=_EqPredicate());
    
    void groupRectangles(std::vector<ncv::Rect>& rectList, std::vector<int>& weights, int groupThreshold, double eps = 0.2);
    void groupRectangles(std::vector<ncv::Rect>& rectList, int groupThreshold, double eps, std::vector<int>* weights, std::vector<double>* levelWeights );
    
    void mixChannels( const ncv::Mat* src, size_t nsrcs, ncv::Mat* dst, size_t ndsts, const int* fromTo, size_t npairs );
    
    NCvSeq* cvMakeSeqHeaderForArray( int seq_flags, int header_size, int elem_size,
                                     const void *array, int total, NCvSeq *seq, NCvSeqBlock * block );
    
    void scalarToRawData(const Scalar& s, void* _buf, int type, int unroll_to);
    void cvInitTreeNodeIterator( NCvTreeNodeIterator* treeIterator, const void* first, int max_level );
    void* cvNextTreeNode( NCvTreeNodeIterator* treeIterator );
    void cvStartReadSeq( const NCvSeq *seq, NCvSeqReader * reader, int reverse );
    void cvChangeSeqBlock( void* _reader, int direction );
    void Line( Mat& img, ncv::Point2i pt1, ncv::Point2i pt2, const void* _color, int connectivity = 8 );
    void cvDrawContours( void* _img, NCvSeq* contour,
                        const unsigned int & _externalColor, const unsigned int & _holeColor,
                        int  maxLevel, int thickness,
                        int line_type, ncv::Point2i _offset );
    void CollectPolyEdges( ncv::Mat& img, const Point2i* v, int count, std::vector<NCvPolyEdge>& edges,
                           const void* color, int line_type, int shift, Point2i offset );
    
    struct CmpEdges
    {
        bool operator ()(const NCvPolyEdge& e1, const NCvPolyEdge& e2)
        {
            return e1.y0 - e2.y0 ? e1.y0 < e2.y0 :
            e1.x - e2.x ? e1.x < e2.x : e1.dx < e2.dx;
        }
    };
    
    void FillEdgeCollection( Mat& img, std::vector<NCvPolyEdge>& edges, const void* color );
    void drawContours( ncv::Mat & image, const std::vector<std::vector<ncv::Point2i> > & _contours,
                       int contourIdx, const unsigned int & color, int thickness, int lineType );
    
    void histPrepareImages( const Mat* images, int nimages, const int* channels,
                            const Mat& mask, int dims, const int* histSize,
                            const float** ranges, bool uniform,
                            std::vector<uchar*>& ptrs, std::vector<int>& deltas,
                            Size& imsize, std::vector<double>& uniranges );
    void calcHistLookupTables_8u( const Mat& hist, int dims, const float** ranges, const double* uniranges,
                                  bool uniform, std::vector<size_t>& _tab );
    void calcHist_8u( std::vector<uchar*>& _ptrs, const std::vector<int>& _deltas,
                      ncv::Size imsize, Mat& hist, int dims, const float** _ranges,
                      const double* _uniranges, bool uniform );
    void calcHist( const Mat* images, int nimages,
                   const int* channels, const Mat &mask,
                   Mat &hist, int dims, const int* histSize,
                   const float** ranges, bool uniform = true, bool accumulate = false );
    float mean( const ncv::Mat & _src, const ncv::Mat & _mask );
    void meanStdDev( const Mat & src, Mat & mean, Mat & sdv, const Mat & mask );
    
    void minMaxIdx(const Mat & src, double* minVal, double* maxVal, const Mat & mask);
    void minMaxLoc( const Mat & img, double* minVal, double* maxVal, const Mat & mask );
    void normalize( const Mat & src, Mat & dst, double a, double b,
                    int norm_type, int rtype, const Mat & mask );
    double compareHist( const Mat & H1, const Mat & H2, int method );
    
    ncv::Rect boundingRect2i( std::vector<ncv::Point2i> & points );
    ncv::Rect boundingRect2f( std::vector<ncv::Point2f> & points );
    
    /* ---- Optical Flow ---- */
    void transpose( const Mat & src, Mat & dst );
    bool solve( Mat & src, Mat & src2arg, Mat & _dst, int method );
    int cvEstimateRigidTransform( const std::vector<Point2f> & matA, const std::vector<Point2f> & matB, Mat& matM, int full_affine );
    
    void sepFilter2D(const Mat & src, Mat & dst, int ddepth,
                     Mat & kernelX, Mat & kernelY,
                     Point2i anchor=Point2i(-1,-1),
                     double delta=0, int borderType=BORDER_DEFAULT );
    
    void fastFree(void* ptr);
    void* fastMalloc( size_t size );
  
    static size_t alignSize(size_t sz, int n)
    {
        return (sz + n-1) & -n;
    }
    
    static inline int clip(int x, int a, int b)
    {
        return x >= a ? (x < b ? x : b-1) : a;
    }
    
    inline  NCvTermCriteria  cvTermCriteria( int type, int max_iter, double epsilon )
    {
        NCvTermCriteria t;
        
        t.type = type;
        t.max_iter = max_iter;
        t.epsilon = (float)epsilon;
        
        return t;
    }
    
    /*!
     Termination criteria in iterative algorithms
     */
    class TermCriteria
    {
    public:
        enum
        {
            COUNT=1, //!< the maximum number of iterations or elements to compute
            MAX_ITER=COUNT, //!< ditto
            EPS=2 //!< the desired accuracy or change in parameters at which the iterative algorithm stops
        };
        
        //! default constructor
        TermCriteria();
        //! full constructor
        TermCriteria(int type, int maxCount, double epsilon);
        //! conversion from CvTermCriteria
        TermCriteria(const NCvTermCriteria& criteria);
        //! conversion to CvTermCriteria
        operator NCvTermCriteria() const;
        
        int type; //!< the type of termination criteria: COUNT, EPS or COUNT + EPS
        int maxCount; // the maximum number of iterations/elements
        double epsilon; // the desired accuracy
    };
    
    inline TermCriteria::TermCriteria() : type(0), maxCount(0), epsilon(0) {}
    inline TermCriteria::TermCriteria(int _type, int _maxCount, double _epsilon)
    : type(_type), maxCount(_maxCount), epsilon(_epsilon) {}
    inline TermCriteria::TermCriteria(const NCvTermCriteria& criteria)
    : type(criteria.type), maxCount(criteria.max_iter), epsilon(criteria.epsilon) {}
    inline TermCriteria::operator NCvTermCriteria() const
    { return cvTermCriteria(type, maxCount, epsilon); }
    
    int buildOpticalFlowPyramid(Mat & _img, std::vector<Mat> & pyramid, Size winSize, int maxLevel, bool withDerivatives=true, int pyrBorder=4, int derivBorder=0, bool tryReuseInputImage=true);
    
    void goodFeaturesToTrack( const Mat & image, std::vector<ncv::Point2f> & corners,
                             int maxCorners, double qualityLevel, double minDistance,
                             Mat & mask, int blockSize=3,
                             bool useHarrisDetector=false, double harrisK=0.04 );
    
    void calcOpticalFlowPyrLK( Mat & _prevImg, Mat & _nextImg,
                              std::vector<Point2f> & _prevPts, std::vector<Point2f> & _nextPts,
                              std::vector<uchar> & _status, std::vector<float> & _err,
                              Size winSize=Size(21,21), int maxLevel=3,
                              TermCriteria criteria=TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 30, 0.01),
                              int flags=0, double minEigThreshold=1e-4);
    
    void fillConvexPoly(Mat& img, const Point2i* pts, int npts,
                        const Scalar & color, int lineType=8,
                        int shift=0);

    Mat estimateRigidTransform( std::vector<ncv::Point2f> & src, std::vector<ncv::Point2f> & dst,
                                    bool fullAffine);
    
    void warpAffine( const Mat & _src, Mat & dst,
                    Mat & M0, Size dsize,
                    int flags = 1, int borderType = 0, const Scalar & borderValue = Scalar() );
    
    void split( Mat& src, std::vector<Mat>& mv);
    void transform( Mat& _src,  Mat& _dst,  Mat& _mtx );
    void transform( std::vector<Point2f>& src,  std::vector<Point2f>& dst,  Mat& m );
    
    void copyMakeBorder( Mat & src, Mat & dst,
                         int top, int bottom, int left, int right,
                         int borderType, const Scalar& value=Scalar());
    
    void findNonZero( const Mat & src, Mat & idx );
    
    //Mat op +/-
    MatExpr operator + (const Mat& a, const Mat& b);
    MatExpr operator - (const Mat& a, const Mat& b);
    
    //Mat op *
    MatExpr operator * (const Mat& a, const Mat& b);
    MatExpr operator * (const Mat& a, double s);
    MatExpr operator * (double s, const Mat& a);
    
    //Mat op <
    Mat operator < (const Mat& a, const int & b);
    
    ncv::Rect& operator &= ( ncv::Rect& a, const ncv::Rect& b );
    ncv::Rect operator & (const ncv::Rect& a, const ncv::Rect& b);
    bool operator == (const ncv::Rect& a, const ncv::Rect& b);

    ncv::Point2f operator - (const Point2f& a, const Point2f& b);
    ncv::Point2i operator - (const Point2i& a, const Point2i& b);
    
    static inline double norm2f(const ncv::Point2f& pt)
    { return sqrt((double)pt.x*pt.x + (double)pt.y*pt.y); }

    static inline double norm2i(const ncv::Point2i& pt)
    { return sqrt((double)pt.x*pt.x + (double)pt.y*pt.y); }
    
    typedef void (*SplitFunc)(const uchar* src, uchar** dst, int len, int cn);
    typedef void (*TransformFunc)( const uchar* src, uchar* dst, const uchar* m, int, int, int );
    typedef int (*SumFunc)(const uchar*, const uchar* mask, uchar*, int, int);
    typedef int (*SumSqrFunc)(const uchar*, const uchar* mask, uchar*, uchar*, int, int);
    typedef void (*MinMaxIdxFunc)(const uchar*, const uchar*, int*, int*, size_t*, size_t*, int, size_t);
    
    typedef void (*RemapFunc)(const Mat& _src, Mat& _dst, const Mat& _xy,
    const Mat& _fxy, const void* _wtab,
    int borderType, const Scalar& _borderValue);
    
    typedef void (*ResizeFunc)( const Mat& src, Mat& dst,
    const int* xofs, const void* alpha,
    const int* yofs, const void* beta,
    int xmin, int xmax, int ksize );
    
    typedef void (*ResizeAreaFastFunc)( const Mat& src, Mat& dst,
    const int* ofs, const int *xofs,
    int scale_x, int scale_y );
    
    typedef void (*ResizeAreaFunc)( const Mat& src, Mat& dst, const DecimateAlpha* xtab, int xtab_size, const DecimateAlpha* ytab, int ytab_size, const int* yofs);
    
    typedef void (*PyrFunc)(const Mat&, Mat&, int);
    
    typedef void (*RemapNNFunc)(const Mat& _src, Mat& _dst, const Mat& _xy,
    int borderType, const Scalar& _borderValue );
}

#endif /* Mat_hpp */
