#ifndef Rcpp__sugar__rep_len_h
#define Rcpp__sugar__rep_len_h

namespace Rcpp{
    namespace sugar{
    
        template <typename eT, typename Expr>
        class Rep_len :
            public SugarVectorExpression<eT,Rep_len<eT,Expr>>,
            public custom_sugar_vector_expression 
        {
        public:
            typedef typename traits::vector_of<eT>::type Vec ;
            
            class const_iterator : public std::iterator_traits<eT*> {
            public:
                typedef typename Rep_len::Vec Vec ;
                
                const_iterator( const Vec& data_, R_xlen_t n_, R_xlen_t index_ ) : 
                    data(data_), n(n_), index(index_), src_index(index % n ){}
                
                const_iterator( const Rep_len& data_, R_xlen_t index_ ) :
                    const_iterator( data_.data, data_.n, index_ ){}
                    
                inline const_iterator& operator++(){ 
                    index++;
                    src_index++ ; if( src_index == n ) src_index = 0 ;
                    return *this ;
                }
                inline const_iterator& operator--(){ 
                    index--;
                    src_index-- ; if( src_index == -1 ) src_index = n-1 ;
                    return *this ;
                }
        
                inline eT operator*() {
                    return data[src_index] ;
                }
                
                inline int operator-( const const_iterator& other ){
                    return index - other.index ;    
                }
                
                inline bool operator==( const const_iterator& other ){ return index == other.index; }
                inline bool operator!=( const const_iterator& other ){ return index != other.index; }
                
                inline const_iterator operator+( R_xlen_t n_ ) const {
                    return const_iterator( data, n, index + n_) ;
                }
                
            private:
                const Vec& data ;
                R_xlen_t n, index, src_index;
                
            } ;
            
            
            Rep_len( const SugarVectorExpression<eT,Expr>& object_, R_xlen_t len_ ) :
                data(object_), len(len_), n(object_.size()){}
        
            inline eT operator[]( R_xlen_t i ) const {
                return data[ i % n ] ;
            }
            inline R_xlen_t size() const { return len ; }
        
            template <typename Target>
            inline void apply( Target& target ) const {
                apply_serial(target) ;    
            }
        
            template <typename Target>
            inline void apply_serial(Target& target) const {
                if (n >= len) {
                    std::copy_n(data.begin(), len, target.begin() );
                    return;
                }
        
                int timesToFullCopy = len / n;
                int leftoverElems = len % n;
                
                // first copy data from the sugar expression
                auto it = target.begin();
                for (R_xlen_t i=0; i < timesToFullCopy; ++i) {
                    std::copy_n(data.begin(), n, it);
                    it += n;
                }
                if (leftoverElems) {
                    std::copy_n(target.begin(), leftoverElems, it);
                }
            }
            
            template <typename Target>
            inline void apply_parallel( Target& target ) const {
                apply_serial(target) ;    
            }
        
            inline const_iterator begin() const { return const_iterator( *this, 0 ) ; }
            inline const_iterator end() const { return const_iterator( *this, size() ) ; }
            
        private:
            Vec data ;
            R_xlen_t len, n ;
        
        } ;
    
    } // sugar
    
    template <typename eT, typename Expr>
    inline sugar::Rep_len<eT,Expr> rep_len( const SugarVectorExpression<eT,Expr>& t, R_xlen_t len ){
        return sugar::Rep_len<eT,Expr>( t, len ) ;
    }


} // Rcpp
#endif

