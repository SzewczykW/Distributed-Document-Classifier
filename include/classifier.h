#ifndef CLASSIFIER_H
#define CLASSIFIER_H

typedef enum {
    DOC_TXT,
    DOC_HTML,
    DOC_TEX,
    DOC_UNKNOWN
} DocType;

DocType classify_file(const char *filepath);
const char *doc_type_to_string(DocType type);

#endif // CLASSIFIER_H

