#ifndef Rcpp_RCPP_API_IMPL_h
#define Rcpp_RCPP_API_IMPL_h

#define RCPP_API_IMPL_NOCONV(__CLASS__)                               \
private:                                                              \
    Storage data ;                                                    \
public:                                                               \
    __CLASS__( const __CLASS__& ) = default ;                         \
    __CLASS__( __CLASS__&& ) = default ;                              \
    __CLASS__& operator=( const __CLASS__& ) = default ;              \
    __CLASS__& operator=( __CLASS__&& ) = default ;                   \
    __CLASS__( SEXP x ){                                              \
        set(x) ;                                                      \
    }                                                                 \
    __CLASS__& operator=( SEXP x){                                    \
        set(x) ;                                                      \
        return *this ;                                                \
    }                                                                 \
    template <typename Proxy>                                         \
    __CLASS__( const GenericProxy<Proxy>& proxy ){                    \
        set(proxy.get()) ;                                            \
    }                                                                 \
    template <typename Proxy>                                         \
    __CLASS__& operator=( const GenericProxy<Proxy>& proxy ){         \
        set(proxy.get()) ;                                            \
        return *this ;                                                \
    }                                                                 \

#define RCPP_API_IMPL(__CLASS__)                                      \
    RCPP_API_IMPL_NOCONV(__CLASS__)                                   \
    inline operator SEXP() const {                                    \
        return data ;                                                 \
    }                                                                 \
    inline AttributeProxy<__CLASS__> attr( const std::string& name ){ \
        return Rcpp::attr(*this, name);                               \
    }                                                                 \
    inline const AttributeProxy<__CLASS__> attr( const std::string& name ) const { \
        return Rcpp::attr(*this, name);                               \
    }                                                                 \
    
#endif    

