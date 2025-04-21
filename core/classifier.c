/**
 * @file classifier.c
 * @brief Implements document type classification based on file extension.
 */

#include "classifier.h"
#include <string.h>

/**
 * @brief Determines the document type based on its file extension.
 *
 * Currently supports:
 * - `.txt` for plain text
 * - `.html` or `.htm` for HTML
 * - `.tex` for LaTeX
 *
 * @param filepath Path to the document file.
 * @return Document type as a value of the DocType enum.
 */
DocType classify_file(const char *filepath)
{
    const char *ext = strrchr(filepath, '.');
    if (!ext)
        return DOC_UNKNOWN;

    if (strcmp(ext, ".txt") == 0)
        return DOC_TXT;
    if (strcmp(ext, ".html") == 0 || strcmp(ext, ".htm") == 0)
        return DOC_HTML;
    if (strcmp(ext, ".tex") == 0)
        return DOC_TEX;

    return DOC_UNKNOWN;
}

/**
 * @brief Converts a document type to its string label.
 *
 * @param type Document type enum.
 * @return Human-readable string representation of the type.
 */
const char *doc_type_to_string(DocType type)
{
    switch (type)
    {
        case DOC_TXT:
            return "TXT";
        case DOC_HTML:
            return "HTML";
        case DOC_TEX:
            return "TEX";
        default:
            return "UNKNOWN";
    }
}

