#include <string.h>
#include "classifier.h"

DocType classify_file(const char *filepath) {
    const char *ext = strrchr(filepath, '.');
    if (!ext) return DOC_UNKNOWN;

    if (strcmp(ext, ".txt") == 0)
        return DOC_TXT;
    else if (strcmp(ext, ".html") == 0 || strcmp(ext, ".htm") == 0)
        return DOC_HTML;
    else if (strcmp(ext, ".tex") == 0)
        return DOC_TEX;
    else
        return DOC_UNKNOWN;
}

const char *doc_type_to_string(DocType type) {
    switch (type) {
        case DOC_TXT: return "TXT";
        case DOC_HTML: return "HTML";
        case DOC_TEX: return "TEX";
        default: return "UNKNOWN";
    }
}

