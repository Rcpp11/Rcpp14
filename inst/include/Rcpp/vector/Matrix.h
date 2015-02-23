#ifndef Rcpp__vector__Matrix_h
#define Rcpp__vector__Matrix_h

namespace Rcpp{

    template <int RTYPE, typename Storage>
    class Matrix : 
        public SugarMatrixExpression< typename Vector<RTYPE>::value_type, Matrix<RTYPE,Storage> >
    {
    private:
        typedef Vector<RTYPE,Storage> Vec ;
        Vec vec ;
        int* dims ;
        
    public:
        typedef typename Vector<RTYPE,Storage>::Proxy Proxy;
        typedef typename Vector<RTYPE,Storage>::iterator iterator;
        typedef typename Vector<RTYPE,Storage>::const_iterator const_iterator;
        
        typedef MatrixColumn<RTYPE, Matrix> Column;
        typedef MatrixRow<RTYPE, Matrix> Row;
        
        Matrix(int nr, int nc) : vec(nr*nc){
            set_dimensions(nr,nc) ;
        }
        
        template <typename U>
        Matrix(int nr, int nc, const U& u) : vec(nr*nc, u){
            set_dimensions(nr,nc) ;
        }
        
        Matrix() : vec(0){
            set_dimensions(0,0) ;
        }
        
        Matrix( SEXP x ){
            SEXP d = Rf_getAttrib(x,R_DimSymbol) ;
            if( d == R_NilValue || Rf_xlength(d) != 2)
                stop("not a matrix") ;
            vec = x ;
            dims = INTEGER(d) ;
        }
        
        Matrix( const Matrix& other ) = default ;
        Matrix& operator=( const Matrix& ) = default ;
            
        template <typename eT, typename Expr>
        Matrix( const SugarMatrixExpression<eT,Expr>& expr ) : vec(expr.nrow() * expr.ncol()) {
            set_dimensions( expr.nrow(), expr.ncol() ) ;
            expr.apply(*this) ;
        }
        
        template <typename eT, typename Expr>
        Matrix& operator=( const SugarMatrixExpression<eT,Expr>& expr ) {
            if( nrow() != expr.nrow() || ncol() != expr.ncol() ) {
                stop("incompatible dimensions, expecting (%d,%d), got (%d,%d)", nrow(), ncol(), expr.nrow(), expr.ncol()) ;
            }
            expr.apply(*this) ;
            return *this ;
        }
        
        inline operator SEXP() const { return vec ; }
            
        inline int nrow() const { return dims[0] ; }
        inline int ncol() const { return dims[1] ; }
        inline R_xlen_t size() const { return vec.size() ; }
        
        inline iterator begin(){ 
            RCPP_DEBUG( "Matrix::begin() = %p", vec.begin() ) ;
            return vec.begin() ; 
        }
        inline iterator end(){ return vec.end(); }
                   
        inline const iterator begin() const {
            RCPP_DEBUG( "Matrix::begin() const = %p", vec.begin() ) ;
            return vec.begin() ; 
        }
        inline const iterator end() const { return vec.end(); }
              
        inline Proxy operator[](R_xlen_t i){ return vec[i] ; }
        inline const Proxy operator[](R_xlen_t i) const{ return vec[i] ; }
        
        inline Proxy operator()(int i, int j) { 
            return vec[offset(i,j)] ; 
        }
        inline const Proxy operator()(int i, int j) const { 
            return const_cast<Vec&>(vec)[offset(i,j)] ;
        }
        
        inline Column column(int i){ return Column(*this, i) ; }
        inline Column operator()(internal::NamedPlaceHolder, int i){ return column(i); }
        
        inline Row row(int i){ return Row(*this, i) ; }
        inline Row operator()(int i, internal::NamedPlaceHolder){ return row(i); }
        
        template <typename T>
        inline Matrix& operator+=( T x ){
            for( auto& val: *this){ 
                val += x; 
            }
            return *this ;
        }
        
        template <typename T>
        inline Matrix& operator*=( T x ){
            for( auto& val: *this){ 
                val *= x; 
            }
            return *this ;
        }
        
        template <typename T>
        inline Matrix& operator/=( T x ){
            for( auto& val: *this){ 
                val /= x; 
            }
            return *this ;
        }
        
        template <typename T>
        inline Matrix& operator-=( T x ){
            for( auto& val: *this){ 
                val -= x; 
            }
            return *this ;
        }

    private:
        
        inline R_xlen_t offset(int i, int j) const {
            return i + nrow()*j ;
        }
        
        inline void set_dimensions(int nr, int nc){
            Shield<SEXP> dim = Rf_allocVector(INTSXP, 2 ) ;
            dims = INTEGER(dim) ;
            dims[0] = nr ;
            dims[1] = nc ;
            Rf_setAttrib(vec, R_DimSymbol, dim ) ;        
        }
        
    } ;
    
    typedef Matrix<CPLXSXP> ComplexMatrix ;  
    typedef Matrix<INTSXP>  IntegerMatrix ;
    typedef Matrix<LGLSXP>  LogicalMatrix ;
    typedef Matrix<REALSXP> NumericMatrix ;
    typedef Matrix<REALSXP> DoubleMatrix ;
    typedef Matrix<RAWSXP>  RawMatrix ;
    
    typedef Matrix<STRSXP>  CharacterMatrix ;   
    typedef Matrix<STRSXP>  StringMatrix ;
    typedef Matrix<VECSXP>  GenericMatrix ;
    typedef Matrix<EXPRSXP> ExpressionMatrix ;
    
}

#endif
