/**
 * @file classifier.h
 * @brief Simple file-type classification based on file extensions.
 */

#ifndef CLASSIFIER_H
#define CLASSIFIER_H

/**
 * @brief Enumeration of supported document types.
 */
typedef enum
{
    DOC_TXT,     /**< Plain text file (.txt) */
    DOC_HTML,    /**< HTML file (.html/.htm) */
    DOC_TEX,     /**< LaTeX file (.tex) */
    DOC_UNKNOWN  /**< Unknown file type */
} DocType;

/**
 * @brief Classifies a file based on its extension.
 *
 * @param filepath Path to the file.
 * @return Detected document type.
 */
DocType classify_file(const char *filepath);

/**
 * @brief Returns a human-readable name for a document type.
 *
 * @param type Document type.
 * @return String label of the type.
 */
const char *doc_type_to_string(DocType type);

#endif

