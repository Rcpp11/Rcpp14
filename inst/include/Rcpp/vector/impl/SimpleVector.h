#ifndef Rcpp__vector__impl_SimpleVector_h
#define Rcpp__vector__impl_SimpleVector_h

namespace Rcpp{

    #undef VEC
    #define VEC Vector<RTYPE,Storage>

    template <int RTYPE, typename Storage>
    class Vector :
        public VectorOfRTYPE<RTYPE>,
        public SugarVectorExpression<typename traits::storage_type<RTYPE>::type, VEC>
    {
    public:
        typedef typename traits::storage_type<RTYPE>::type value_type ;
        typedef value_type&        reference       ;
        typedef const value_type&  const_reference ;
        typedef reference          Proxy           ;
        typedef const_reference    const_Proxy     ;
        typedef value_type*        iterator        ;
        typedef const value_type*  const_iterator  ;

        #include <Rcpp/vector/impl/RCPP_VECTOR_API.h>

    public:
        Vector( R_xlen_t n, value_type x ) {
            reset(n);
            std::fill( begin(), end(), x) ;
        }

        Vector( std::initializer_list<value_type> list ){
            reset(list.size());
            std::copy( list.begin(), list.end(), begin() ) ;
        }

        Vector( std::initializer_list<traits::named_object<value_type>> list ){
            int n = list.size() ;
            reset(n);
            auto input_it = list.begin() ;
            auto it = begin() ;
            SEXP nams = PROTECT(Rf_allocVector(STRSXP, n) ) ;
            for( int i=0; i<n; i++, ++it, ++input_it){
                SET_STRING_ELT(nams, i, PRINTNAME(input_it->name) ) ;
                *it = input_it->object ;
            }
            UNPROTECT(1) ;
            names(*this) = nams ;
        }


        inline iterator begin() { return dataptr() ; }
        inline iterator end() { return dataptr() + size() ; }

        inline const_iterator begin() const{ return dataptr() ; }
        inline const_iterator end() const{ return dataptr() + size() ; }

        inline reference operator[](R_xlen_t i){
            RCPP_CHECK_BOUNDS(i)
            return dataptr()[i] ;
        }
        inline const_reference operator[](R_xlen_t i) const {
            RCPP_CHECK_BOUNDS(i)
            return dataptr()[i] ;
        }

    } ;

    #undef VEC

}


#endif
