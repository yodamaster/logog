/*!
 * \file statics.hpp Static variables simultaneously assigned for all instances of logog.
 */

#ifndef __LOGOG_STATICS_HPP__
#define __LOGOG_STATICS_HPP__


namespace logog
{

class Timer;
class Formatter;
class Target;
class Mutex;

static void DestroyAllNodes();
static void DestroyGlobalTimer();
static void DestroyDefaultFormatter();
static void DestroyStringSearchMutex();
static void DestroyMessageCreationMutex();

/* Technically this information should be in node.hpp but statics is responsible for
 * this global list.
 */
class Statics
{
public:
    /** A pointer to the malloc() compatible function used by logog.  See logog::Initialize()
     ** for more details. */
    void *(*s_pfMalloc)( size_t );
    /** A pointer to the free() compatible function used by logog.  See logog::Initialize()
     ** for more details. */
    void (*s_pfFree)( void * );
    /** Pointers to all the currently existing nodes in the network. */
    void *s_pAllNodes;
    /** Pointers to only those nodes that are capable of subscribing. */
    void *s_pAllSubscriberNodes;
    /** Pointers to only those nodes that are capable of subscribing and publishing. */
    void *s_pAllFilterNodes;
    /** Pointers to the group of all valid targets. */
    void *s_pAllTargets;
    /** Pointer to the default filter, if any. */
    void *s_pDefaultFilter;
    /** The default global shared timer.  All events are generally in reference to this timer, though yoy may create your own timers. */
    Timer *s_pTimer;
    /** A lock on the KMP search for all strings.  Prevents mutex explosions.  */
    void *s_pStringSearchMutex;
    /** A lock for creating messages.  Prevents dual message creation from multiple threads. */
    Mutex *s_pMessageCreationMutex;
    /** The default Formatter for all targets.  Targets may use their individual formatters as well if preferred. */
    Formatter *s_pDefaultFormatter;
    /** The number of sockets created. */
    int s_nSockets;
    /** A pointer to this object; used for final destruction. */
    Statics *s_pSelf;
    Statics()
    {
        s_pAllNodes = NULL;
        s_pAllSubscriberNodes = NULL;
        s_pAllFilterNodes = NULL;
        s_pAllTargets = NULL;
        s_pTimer = NULL;
        s_pDefaultFormatter = NULL;
        s_pDefaultFilter = NULL;
        s_pStringSearchMutex = NULL;
        s_pMessageCreationMutex = NULL;
        s_pfMalloc = NULL;
        s_pfFree = NULL;
        s_pSelf = this;
        s_nSockets = 0;
    }

    /** Resets all statics to startup values.  Releases memory allocated by statics. */
    void Reset()
    {
        DestroyGlobalTimer();
        DestroyDefaultFormatter();
        s_pDefaultFilter = NULL; // This will be destroyed on the next step
        DestroyAllNodes();
        DestroyStringSearchMutex();
        DestroyMessageCreationMutex();
        s_pfMalloc = NULL;
        s_pfFree = NULL;
        s_nSockets = 0;
    }

    ~Statics()
    {
        Reset();
    }
};

Statics &Static()
{
    static Statics *pStatics = new Statics();
    return *pStatics;
}

/** Destroys the Static() structure.  Calls to Static() after calling DestroyStatic() will probably crash your
 ** program.
 */
void DestroyStatic()
{
    Statics *pStatics = &Static();
    delete pStatics;
}
}
#endif // __LOGOG_STATICS_HPP
