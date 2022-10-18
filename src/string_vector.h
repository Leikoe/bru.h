//
// Created by Linkenz on 18/10/2022.
//

#ifndef BRU_H_STRING_VECTOR_H
#define BRU_H_STRING_VECTOR_H

struct StringVector {
    size_t capacity;
    size_t size;
    char ** strings;  // dynamic array of pointers
};

void string_vector_init(struct StringVector * this, size_t capacity);
void string_vector_free(struct StringVector * this);

void string_vector_add(struct StringVector * this,
const char *start,
const char *end);
size_t string_vector_size(const struct StringVector * this);
char * string_vector_get(const struct StringVector * this,
size_t index);

struct StringVector split_line(char * line)

#endif //BRU_H_STRING_VECTOR_H
