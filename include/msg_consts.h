/**
 * @file msg_consts.h
 * @author Wiktor Szewczyk
 * @brief Constants for MPI messaging and buffer sizes.
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

/// MPI message tags
#define REQUEST_MSG 0  ///< Worker requests a new task
#define FILE_MSG    1  ///< Filename message
#define VEC_MSG     2  ///< Feature vector message
#define DONE_MSG    3  ///< Worker signals completion

#endif

