/*****************************************************************************
*                                                                             
*   GameConsts.h
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: The collection of the game constant parameters in use.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/

#pragma warning (disable : 4239) 
    // warning C4239: nonstandard extension used : conversion from 'class XXX' to 'class XXX &'
#pragma warning (disable : 4201) 
    // warning C4201: nonstandard extension used : nameless struct/union


#ifndef GAME_CONSTS_H
#define GAME_CONSTS_H
    
#define MAX_TANKS                    4                  // Max number of tanks in one game
#define MAX_POSSIBLE_OBJECTS       100                  // Max number of objects that appear concurrently

    // Renderer:
#define MIN_RENDER_FREQ              1                  // Minimal rendering rate (frames per second)
#define MAX_RENDER_FREQ             40                  // Maximal rendering rate (frames per second)
#define DEFAULT_RENDER_FREQ         20                  // Default rendering rate (frames per second)

    // Map:
#define MAP_WIDTH                  500                  // Width (in pixels) of game map
#define MAP_HEIGHT                 500                  // Height (in pixels) of game map
#define MAP_WIDTH_BITS               9                  // Number of bits to hold map width
#define MAP_HEIGHT_BITS              9                  // Number of bits to hold map height
#define SECTOR_DIVISION              4                  // Each axis is divided SECTOR_DIVISION times

                                                        // Maximal sector number (e.g: 0..15 = 4 x 4 division)        
#define MAX_SECTOR       ((SECTOR_DIVISION) * (SECTOR_DIVISION) - 1)          
#define MAP_WIDTH_BITS               9                  // Number of bits to hold map width
#define MAP_HEIGHT_BITS              9                  // Number of bits to hold map height
#define MAX_MINES_PER_SECTOR        (MAP_WIDTH  / SECTOR_DIVISION / MINE_WIDTH )   *  \
                                    (MAP_HEIGHT / SECTOR_DIVISION / MINE_HEIGHT)   *  2

    // Orientation
#define MAX_DIRECTIONS              24                  // Possible rotations (15 degrees)
#define INVALID_DIRECTION           (MAX_DIRECTIONS + 1)
                                                        // Indicates invalid direction index
    // Tank selection animation:
#define TANK_ANIM_WIDTH            110                  // Width (in pixels) of tank animation AVI
#define TANK_ANIM_HEIGHT            70                  // Height (in pixels) of tank animation AVI

    // Status bars:
#define SHIELD_STATUS_WIDTH         20                  // Width of shield status bar
#define AMMO_STATUS_WIDTH           50                  // Width of ammunition status bar
#define AMMO_STATUS_HEIGHT         220                  // Height of ammunition status bar
#define DIGIT_WIDTH                  9                  // Width of single digit (ammo. status bar)
#define DIGIT_HEIGHT                13                  // Height of single digit (ammo. status bar)
#define MAX_STATUS_VALUE           500                  // Max value of status counters

    // Transparency:
#define TRANSP_COLOR                10

    // Mine object:
#define MINE_WIDTH                  10                  // Width of mine object
#define MINE_HEIGHT                 10                  // Height of mine object
#define MINE_INTENSITY              40                  // Constant explosion intensity of a mine
#define MINE_EXPIRATION   DWORD(120000)                 // Mine time-out (2 minutes)

    // Bullet object:
#define BULLET_WIDTH                 3                  // Width of bullet object
#define BULLET_HEIGHT                3                  // Height of bullet object
#define BULLET_SPEED                80                  // Pixels / second
#define BULLET_INTENSITY             7                  // Constant explosion intensity of a bullet
#define BULLET_DISTANCE_SQR      10000                  // Max distance of a bullet (squared)

    // Shell object:
#define SHELL_WIDTH                  8                  // Width of shell object
#define SHELL_HEIGHT                 8                  // Height of shell object
#define SHELL_SPEED                100                  // Pixels / second
#define SHELL_INTENSITY             40                  // Maximal explosion intensity of a shell
#define MIN_SHELL_RADIUS            20                  // Minimal explosion radius 
                                                        // (below this distance you get a full blast power)
#define MAX_SHELL_RADIUS            40                  // Maximal explosion radius 
                                                        // (above this distance you don't feel a thing)
    // Bomber object
#define BOMBER_WIDTH                50                  // Width of bomber image
#define BOMBER_HEIGHT               50                  // Height of bomber image
#define BOMBER_SPEED                40                  // Speed of bomber (pixels / sec)
#define BOMBS_TIME_GAP            2000                  // Time gap between bombs (msecs)
#define BOMBER_ROTATION_DELAY       25                  // Millisecs between rotation / movement in setup mode
#define EDGE_TICKS                  50                  // Number of setup positions on a single map edge
 
    // Bomb object:
#define BOMB_WIDTH                  50                  // Width of bomb object
#define BOMB_HEIGHT                 50                  // Height of bomb object
#define BOMB_INTENSITY              50                  // Maximal explosion intensity of a bomb
#define BOMB_SPEED                  15                  // Bomb speed equals the bomber's speed
#define MIN_BOMB_RADIUS             20                  // Minimal explosion radius
#define MAX_BOMB_RADIUS            100                  // Maximal explosion radius

    // Tank object:
#define TANK_BLOCK_WIDTH             4                  // Width of tank in map blocks
#define TANK_BLOCK_HEIGHT            4                  // Height of tank in map blocks
#define TANK_WIDTH                  40                  // Width of tank in pixels
#define TANK_HEIGHT                 40                  // Height of tank in pixels
#define TANK_INIT_SHELLS            40                  // Number of shells a tank begins with
#define TANK_INIT_BULLETS          250                  // Number of bullets a tank begins with
#define TANK_INIT_MINES              5                  // Number of mines a tank begins with
#define TANK_INIT_SHIELD_LEVEL     100                  // Quantity of initial shield level
#define TANK_BONUS_SHELLS           10                  // Number of shells granted by a bonus object
#define TANK_BONUS_BULLETS          50                  // Number of bullets granted by a bonus object
#define TANK_BONUS_MINES             2                  // Number of mines granted by a bonus object
#define TANK_BONUS_SHIELD           30                  // Quantity of shield granted by a bonus object
#define BULLET_FIRE_RATE             3                  // Number of bullets per sec.
#define SHELL_FIRE_RATE              1                  // Number of shells per sec.
#define MINE_FIRE_DELAY            600                  // We allow less than 2 mines per sec.
#define TANK_ROTATION_DELAY        108                  // Millisecs between 15 degrees rotations
#define FIRE_RATE_BONUS              2                  // Factor of fire rate when bonus is effective
#define FIRE_RATE_BONUS_DURATION 25000                  // Duration of fire rate bonus in msec
#define TANK_MAX_VELOCITY   double( 40.0 )              // Tank's velocity in pixel per sec. 
#define TANK_75_VELOCITY    double(TANK_MAX_VELOCITY * 0.75)
#define TANK_50_VELOCITY    double(TANK_MAX_VELOCITY * 0.50)
#define TANK_25_VELOCITY    double(TANK_MAX_VELOCITY * 0.25)
#define MAX_POS_TABLE               30                  // Table of last 30 tanks positions
#define MAX_BAD_RTT_COUNT            5                  // After X continous bad RTTs, 
                                                        // a tank becomes zombie, and vice versa
#define MAX_VALID_RTT             1500                  // Maximal Round Trip Time considered valid

    // Bonus object:
#define BONUS_WIDTH                 16                  // Width of bonus object
#define BONUS_HEIGHT                16                  // Height of bonus object
#define BONUS_MIN_LIFESPAN        6000                  // Minimal life span of a bonus
#define BONUS_LIFESPAN_RANGE      3000                  // Life span range for rand func.

    // Game-over animation
#define GAMEOVER_ANIM_WIDTH        400
#define GAMEOVER_ANIM_HEIGHT        64

    // Communication object:
#define COMM_TIME_OUT           600000                  // Time out for wait event
#define CHKSUM_TIME_GAP_BITS        10                  // Time gap (msecs) between checksum is 2 to the power ot this.
#define MAX_COMM_MUTE_PERIOD      5000                  // Max period of time the host didn't hear from a tank
                                                        // before it becomes zombie
#define MAX_PLAYER_NAME             10                  // Max length of a player name
#define MAX_CHAT_MSG_LEN            30                  // Max length of a chat msg

/************  Monitoring definitions and macros ****************/

/*  Define NET_MON_GAME to allow network monitoring of game messages: 
    All incoming and outgiong game messages will
    be TRACEd (excluding chksums).
*/
//#define NET_MON_GAME    


/*  Define NET_MON_SYNC to allow network monitoring of game synchronization messages: 
    All incoming and outgiong game synchronization messages will
    be TRACEd (chksums and checksum checking results).
*/
//#define NET_MON_SYNC    


/*  Define NET_MON_SYS to allow network monitoring of DPlay system messages: 
    All incoming and outgiong DPlay system messages will
    be TRACEd.
*/
#define NET_MON_SYS  


/* Trace game message */
#ifdef NET_MON_GAME
    #define NET_GAME_TRACE(x) {TRACE ("\tGame message:"); TRACE x; TRACE ("\n");}
#else
    #define NET_GAME_TRACE(x)
#endif

/* Trace game synch message */
#ifdef NET_MON_SYNC
    #define NET_SYNC_TRACE(x) {TRACE ("\tGame synch message:"); TRACE x; TRACE ("\n");}
#else
    #define NET_SYNC_TRACE(x)
#endif

/* Trace DirectPlay system message */
#ifdef NET_MON_SYS                                              
    #define NET_SYS_TRACE(x) {TRACE ("\tDPlay system message:"); TRACE x; TRACE ("\n");}
#else
    #define NET_SYS_TRACE(x)
#endif

/************  Statistics definitions ****************/

/*  Define GATHER_RENDERING_STATS to allow the system to collect
    statistics about the rendering performance (frame per second,
    free time in each frame, etc.).
*/
#define GATHER_RENDERING_STATS

/*  Define GATHER_NETWORK_STATS to allow the system to collect
    statistics about the network performance (bytes send / received per second,
    round trip times and propogation delay).
*/
#define GATHER_NETWORK_STATS

/*  Define GATHER_SYNC_STATS to allow the system to collect
    statistics about the synchronization system performance 
    (checksum mismatch rate etc.)
*/
#define GATHER_SYNC_STATS

/************  General definitions ****************/

/*  Define REAL_RANDOM to make the random elements of the game completely random.
    If you disable real randomness, each activation of the program will yield the same
    random numbers (good for debugging)
*/
#define REAL_RANDOM   

/*  Use the following two definitions to manage the priorities of the 
    game manager threrad (renderer) and the communication manager thread.
    The main application thread (the GUI thread) remains in THREAD_PRIORITY_NORMAL
*/
#define GAME_MANAGER_THREAD_PRIORITY        THREAD_PRIORITY_HIGHEST 
#define COMM_MANAGER_THREAD_PRIORITY        THREAD_PRIORITY_HIGHEST


// Default keyboard assignments
const int DEFAULT_KEYS_SETTINGS[] = {
        VK_RIGHT,           // Turn Right
        VK_LEFT,            // Turn Left
        VK_UP,              // Forward
        VK_DOWN,            // BackWard
        VK_CONTROL,         // Fire Shell
        VK_RETURN,          // Fire Bullet
        'M',                // Drop Mine
        'A' };              // Aerial Support

#define MAX_DWORD       0xFFFFFFFF

#if !defined ( UNREFERENCED_PARAMETER )
    #ifdef _DEBUG
        #define UNREFERENCED_PARAMETER(P)
    #else
        #define UNREFERENCED_PARAMETER(P)   P
    #endif
#endif // !defined ( UNREFERENCED_PARAMETER )



#endif  // GAME_CONSTS_H
