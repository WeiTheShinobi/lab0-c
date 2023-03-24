#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *h = malloc(sizeof(struct list_head));
    if (!h)
        return NULL;
    INIT_LIST_HEAD(h);
    return h;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    if (!l)
        return;

    struct list_head *curr = l->next;
    while (curr != l) {
        curr = curr->next;
        q_release_element(list_entry(curr->prev, element_t, list));
    }

    free(l);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head)
        return false;

    element_t *e = malloc(sizeof(element_t));
    if (!e)
        return false;

    int len = strlen(s);
    e->value = malloc(sizeof(char) * (len + 1));
    if (!e->value) {
        free(e);
        return false;
    }

    strncpy(e->value, s, len + 1);
    list_add(&e->list, head);

    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;

    element_t *e = malloc(sizeof(element_t));
    if (!e)
        return false;

    int len = strlen(s);
    e->value = malloc(sizeof(char) * (len + 1));

    if (!e->value) {
        free(e);
        return false;
    }

    strncpy(e->value, s, len + 1);
    list_add_tail(&e->list, head);

    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head)) {
        return NULL;
    }

    element_t *e = list_entry(head->next, element_t, list);
    if (sp || bufsize) {
        strncpy(sp, e->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    list_del(&e->list);
    return e;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head)) {
        return NULL;
    }

    element_t *e = list_entry(head->prev, element_t, list);
    if (sp || bufsize) {
        strncpy(sp, e->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    list_del(&e->list);
    return e;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head || list_empty(head))
        return 0;

    int len = 0;
    struct list_head *li;

    list_for_each (li, head)
        len++;

    return len;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    struct list_head **pp = &head;

    for (struct list_head *fast = head->next;
         fast != head && fast->next != head; fast = fast->next->next) {
        pp = &(*pp)->next;
    }
    element_t *e = list_entry((*pp)->next, element_t, list);
    list_del(&e->list);
    q_release_element(e);

    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (!head)
        return false;
    if (list_empty(head) || list_is_singular(head))
        return true;

    element_t *curr, *next;
    bool isDup = false;

    list_for_each_entry_safe (curr, next, head, list) {
        if (&next->list != head && !strcmp(curr->value, next->value)) {
            list_del(&curr->list);
            q_release_element(curr);
            isDup = true;
        } else if (isDup) {
            list_del(&curr->list);
            q_release_element(curr);
            isDup = false;
        }
    }

    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (!head || list_empty(head))
        return;

    for (struct list_head *first = head->next, *second = head->next->next;
         first != head && second != head;
         first = first->next, second = first->next) {
        first->prev->next = second;
        second->prev = first->prev;

        first->next = second->next;
        first->prev = second;

        second->next = first;
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head))
        return;

    struct list_head *first = head, *second = head->next;
    do {
        first->next = first->prev;
        first->prev = second;

        first = second;
        second = first->next;
    } while (first != head);
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    struct list_head *curr, *next, *tmp = head;
    LIST_HEAD(new_head);

    int num = 0;
    list_for_each_safe (curr, next, head) {
        ++num;
        if (num == k) {
            list_cut_position(&new_head, tmp, curr);
            q_reverse(&new_head);
            list_splice_init(&new_head, tmp);
            num = 0;
            tmp = next->prev;
        }
    }
}

/* Sort elements of queue in ascending order */
void q_sort(struct list_head *head) {}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending order */
int q_merge(struct list_head *head)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
