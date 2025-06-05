/* cache type */
enum cache_name = {L1I, L1D, L2}	
enum cache_policy_t = {LRU, FIFO, RAND}
enum cache_log_t = {O, LV1, LV2}
/* cache log */
let cache_log : bool = true
let cache_trace : bool = false
let cache_log_type : cache_log_t = LV2

/* cache config */
/* common cache config */
let cache_policy = RAND
type linesz_t : Int = 32/* byte */
/* l1i cache config */
type l1i_sets_t : Int = 512
type l1i_ways_t : Int = 2
/* l1d cache config */
type l1d_sets_t : Int = 512
type l1d_ways_t : Int = 2
/* l2 cache config */
type l2_sets_t : Int = 512
type l2_ways_t : Int = 16

/* 3
L1D 16kb,  block size 32byte, set 512,  way 1 
L1I 16kb,  block size 32byte, set 512,  way 1 
L2  256kb, block size 32byte, set 512, way 16
32 512 1 512 1 512 16 
*/

/*
L1D 16kb,  block size 32byte, set 256,  way 2 
L1I 16kb,  block size 32byte, set 256,  way 2 
L2  256kb, block size 32byte, set 512, way 16
32 256 2 256 2 512 16 
*/

/*
L1D 16kb,  block size 32byte, set 128,  way 4 
L1I 16kb,  block size 32byte, set 128,  way 4 
L2  256kb, block size 32byte, set 512, way 16
32 128 4 128 4 512 16 
*/

/*
L1D 16kb,  block size 32byte, set 64,  way 8 
L1I 16kb,  block size 32byte, set 64,  way 8 
L2  256kb, block size 32byte, set 512, way 16
32 64 8 64 8 512 16 
*/

/* 1
L1D 4kb,  block size 32byte, set 128,  way 1 
L1I 4kb,  block size 32byte, set 128,  way 1 
L2  256kb, block size 32byte, set 512, way 16
32 128 1 128 1 512 16 
*/
/* 
L1D 8kb,  block size 32byte, set 256,  way 1 
L1I 8kb,  block size 32byte, set 256,  way 1 
L2  256kb, block size 32byte, set 512, way 16
32 256 1 256 1 512 16 
*/
/* 
L1D 16kb,  block size 32byte, set 512,  way 1 
L1I 16kb,  block size 32byte, set 512,  way 1 
L2  256kb, block size 32byte, set 512, way 16
32 512 1 512 1 512 16 
*/
/* 
L1D 32kb,  block size 32byte, set 1024,  way 1 
L1I 32kb,  block size 32byte, set 1024,  way 1 
L2  256kb, block size 32byte, set 512, way 16
32 1024 1 1024 1 512 16 
*/
