/**
 * @file msg_consts.h
 * @author Wiktor Szewczyk
 * @brief Constants for GASPI messaging and buffer sizes.
 */

#ifndef MSG_CONSTS_H
#define MSG_CONSTS_H

/// Maximum size of a document in bytes
#define MAX_DOC_SIZE 4096

/// Maximum number of dictionary keywords
#define MAX_KEYWORDS 256

/// Maximum length of a single keyword
#define MAX_WORD_LEN 64

/// Maximum number of files
#define MAX_FILES 1024

/// GASPI notification values
#define SHUTDOWN_NOTIF 0       ///< Shutdown signal
#define WORK_AVAILABLE_NOTIF 1 ///< Work available notification
#define WORK_DONE_NOTIF 2      ///< Work completed notification

/// GASPI notification IDs
#define WORK_NOTIF_ID 0   ///< Work notification ID
#define RESULT_NOTIF_ID 1 ///< Result notification ID

#endif
