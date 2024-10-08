#pragma once
#include <stdint.h>

// clang-format off

#if defined(DISCORD_DYNAMIC_LIB)
#  if defined(_WIN32)
#    if defined(DISCORD_BUILDING_SDK)
#      define DISCORD_EXPORT __declspec(dllexport)
#    else
#      define DISCORD_EXPORT __declspec(dllimport)
#    endif
#  else
#    define DISCORD_EXPORT __attribute__((visibility("default")))
#  endif
#else
#  define DISCORD_EXPORT
#endif

// clang-format on

#ifdef __cplusplus
extern "C" {
#endif

// Only PLAYING, LISTENING and WATCHING can be used by RPC.
enum ActivityType {
    PLAYING = 0,
    STREAMING = 1,
    LISTENING = 2,
    WATCHING = 3,
    CUSTOM = 4,
    COMPETING = 5
};

typedef struct DiscordRichPresence {
    int64_t type;
    const char* state;   /* max 128 bytes */
    const char* details; /* max 128 bytes */
    int64_t startTimestamp;
    int64_t endTimestamp;
    const char* button1name;
    const char* button1link;
    const char* button2name;
    const char* button2link;
    const char* largeImageKey;  
    const char* largeImageText; /* max 128 bytes */
    const char* smallImageKey;  
    const char* smallImageText; /* max 128 bytes */
    const char* partyId;        /* max 128 bytes */
    int partySize;
    int partyMax;
    int partyPrivacy;
    const char* matchSecret;    /* max 128 bytes */
    const char* joinSecret;     /* max 128 bytes */
    const char* spectateSecret; /* max 128 bytes */
    int8_t instance;
} DiscordRichPresence;

typedef struct DiscordUser {
    const char* userId;
    const char* username;
    const char* discriminator;
    const char* avatar;
} DiscordUser;

typedef struct DiscordEventHandlers {
    void (*ready)(const DiscordUser* request);
    void (*disconnected)(int errorCode, const char* message);
    void (*errored)(int errorCode, const char* message);
    void (*joinGame)(const char* joinSecret);
    void (*spectateGame)(const char* spectateSecret);
    void (*joinRequest)(const DiscordUser* request);
} DiscordEventHandlers;

#define DISCORD_REPLY_NO 0
#define DISCORD_REPLY_YES 1
#define DISCORD_REPLY_IGNORE 2
#define DISCORD_PARTY_PRIVATE 0
#define DISCORD_PARTY_PUBLIC 1

DISCORD_EXPORT void Discord_Initialize(const char* applicationId,
                                       DiscordEventHandlers* handlers,
                                       bool autoRegister = false,
                                       const char* optionalSteamId = "");
DISCORD_EXPORT void Discord_Shutdown(void);

/* checks for incoming messages, dispatches callbacks */
DISCORD_EXPORT void Discord_RunCallbacks(void);

/* If you disable the lib starting its own io thread, you'll need to call this from your own */
#ifdef DISCORD_DISABLE_IO_THREAD
DISCORD_EXPORT void Discord_UpdateConnection(void);
#endif
DISCORD_EXPORT bool Discord_IsConnected();
DISCORD_EXPORT void Discord_UpdatePresence(const DiscordRichPresence* presence);
DISCORD_EXPORT void Discord_ClearPresence(void);

DISCORD_EXPORT void Discord_Respond(const char* userid, /* DISCORD_REPLY_ */ int reply);

DISCORD_EXPORT void Discord_UpdateHandlers(DiscordEventHandlers* handlers);

#ifdef __cplusplus
} /* extern "C" */
#endif
