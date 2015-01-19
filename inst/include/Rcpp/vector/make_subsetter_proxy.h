#ifndef RCPP_VECTOR_MAKE_SUBSETTER_PROXY_H
#define RCPP_VECTOR_MAKE_SUBSETTER_PROXY_H

namespace Rcpp {
    
    template < typename Source, typename Expr >
    inline auto make_subset_proxy( Source& source, const SugarVectorExpression<int, Expr>& index ){
        return SubsetProxy<Source,int,Expr,false>( source, index ) ;   
    }
    
    template < typename Source, typename Expr >
    inline auto make_subset_proxy( Source& source, const SugarVectorExpression<String, Expr>& index ){
        auto n = index.size() ;
        IntegerVector ind(n) ;
        SEXP names = Rf_getAttrib( source, R_NamesSymbol ) ;
        SEXP* names_it = reinterpret_cast<SEXP*>(DATAPTR(names)) ;
        SEXP* end = names_it + source.size() ;
        auto index_it = sugar_begin(index) ;
        for( int i=0; i<n; i++){
            SEXP s = index_it[i] ;
            auto ix = std::find( names_it, names_it + source.size() , s ) ;
            if( ix == end ) stop( "name not found" ) ;
            ind[i] = (ix - names_it) ;
        }
        return SubsetProxy<Source,int,IntegerVector,true>( source, ind ) ;   
    }
    
    template < typename Source, typename Expr >
    inline auto make_subset_proxy( Source& source, const SugarVectorExpression<Rboolean, Expr>& index ){
        auto n = index.size() ;
        if( n != source.size() ) 
            stop( "logical index size incompatible with source size (%d != %d)", n, source.size() ) ;
        auto m = sum(index) ;
        IntegerVector ind(m) ;
        auto it = sugar_begin(index) ;
        for( int i=0, k=0; i<n ; i++, ++it){
            if( *it ) ind[k++] = i ;
        }
        return SubsetProxy<Source,int,IntegerVector,true>( source, ind ) ;   
    }
    
    template < typename Source, typename Expr >
    inline auto make_subset_proxy( Source& source, const SugarVectorExpression<bool, Expr>& index ){
        auto n = index.size() ;
        if( n != source.size() ) 
            stop( "logical index size incompatible with source size (%d != %d)", n, source.size() ) ;
        auto m = sum(index) ;
        IntegerVector ind(m) ;
        auto it = sugar_begin(index) ;
        for( int i=0, k=0; i<n ; i++, ++it){
            if( *it ) ind[k++] = i ;
        }
        return SubsetProxy<Source,int,IntegerVector,true>( source, ind ) ;   
    }
    
    
}

#endif
