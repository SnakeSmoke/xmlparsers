#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "xml-parser.h"

void print_string_pointer_position()
{
  printf("\n_________________\n");
  printf("Current s -> %s", s);
  printf("\n_________________\n");
}

void ignore_special_chars()
{
  while (!isalpha(*s))
  {
    s++;
  }
}

void push(ioopm_stack_t *s, xml_node_t *pushed_node)
{
  node_t *n = calloc(1, sizeof(node_t));
  n->elem = pushed_node;
  n->next = NULL;

  if (s->size == 0)
  {
    s->top = n;
  }
  else
  {
    n->next = s->top;
    s->top = n;
  }
  s->size++;
}

char *pop(ioopm_stack_t *s)
{
  node_t *old_top = s->top;
  s->top = old_top->next;
  char *poped_value = old_top->elem->text;
  free(old_top);
  s->size--;
  return poped_value;
}

void destroy_stack(ioopm_stack_t *s)
{
  while (s->size != 0)
  {
    pop(s);
  }
  free(s);
}

void print_stack(ioopm_stack_t *s)
{
  node_t *iter = s->top;

  printf("STACK SIZE! -> %d\n", s->size);
  printf("CURRENT STACK!\n");
  while (iter != NULL)
  {
    printf("%s - ", iter->elem->text);
    iter = iter->next;
  }
}

bool element_is_in_stack(ioopm_stack_t *s, char *elem)
{
  node_t *iter = s->top;

  while (iter != NULL)
  {
    //printf("\niter -> %s\n", iter->elem);
    if (strcmp(iter->elem->text, elem) == 0)
    {
      return true;
    }
    iter = iter->next;
  }
  return false;
}

xml_node_t *new_element_node(xml_parse_tree_t *t)
{
  xml_node_t *n = calloc(1, sizeof(xml_node_t));
  n->text = parse_element_name();
  n->type = ELEMENT;
  n->status = OPEN;

  if (t->size == 0)
  {
    t->root = n;
  }
  else
  {
    if (t->current->type == TEXT)
    {
      t->current->next = n;
    }
  }

  t->current = n;
  t->size++;
  //printf("\n current node -> %s, status -> %d\n", t->current->text, t->current->status);
  return n;
}

void find_xml_in_stack_and_close(ioopm_stack_t *s, xml_parse_tree_t *t, char *elem)
{
  node_t *cursor = s->top;
  while (cursor != NULL)
  {
    if (strcmp(cursor->elem->text, elem) == 0)
    {
      cursor->elem->status = CLOSED;
      t->current = cursor->elem;

      return;
    }
    cursor = cursor->next;
  }
}

ioopm_stack_t *create_stack()
{
  ioopm_stack_t *s = calloc(1, sizeof(ioopm_stack_t));
  s->size = 0;
  s->top = NULL;
  return s;
}

xml_parse_tree_t *create_xml_parse_tree()
{
  xml_parse_tree_t *t = calloc(1, sizeof(xml_parse_tree_t));
  t->root = NULL;
  t->current = NULL;
  t->size = 0;
  return t;
}

xml_node_t *parse_element()
{
  ioopm_stack_t *stack = create_stack();
  xml_parse_tree_t *t = create_xml_parse_tree();

  if (strstr(s, "<>"))
  {
    return NULL;
  }

  while (*s)
  {

    if (*s == '<')
    {
      s++;
      if (*s == '/')
      {
        //print_string_pointer_position();
        s++;

        char *closing_tag = parse_element_name();
        //printf("closing_tag -> %s\n", closing_tag);
        if (!element_is_in_stack(stack, closing_tag))
        {
          return NULL;
        }
        find_xml_in_stack_and_close(stack, t, closing_tag);
      }
      else if (isalpha(*s))
      {
        xml_node_t *element_node = new_element_node(t);
        push(stack, element_node);
      }
    }

    if (*s == '>')
    {
      s++;

      if (!isalpha(*s))
      {
        s--;
      }
      else
      {
        xml_node_t *text_node = parse_text();
        if (t->current->status == OPEN)
        {
          //printf("IF OPEN: Current t -> %s\n", t->current->text);
          t->current->subnode = text_node;
          //printf("IF OPEN: Current t subtree -> %s\n", t->current->subnode->text);
        }
        else
        {
          //printf("ELSE CLOSED: Current t -> %s\n", t->current->text);
          t->current->next = text_node;
          //printf("ELSE CLOSED: Current t next -> %s\n", t->current->next->text);
        }

        t->current = text_node;
        t->size++;
        s--;
      }
    }
    s++;
    //printf("\n current node -> %s, status -> %d\n", t->current->text, t->current->status);
  }

  destroy_stack(stack);
  return t->root;
}

// free_xml_tree should take a parse tree and free all allocated memory.

void free_xml_tree(xml_node_t *xml)
{
  free(xml);
}

// parse_whitespace will simply skip over any whitespace. Always succeeds.

bool parse_whitespace()
{
  while (*s == ' ' || *s == '\t' || *s == '\n')
  {
    s++;
  }
  return true;
}

// parse_text parses text inside elements. It should return a TEXT node with
// text up to but not including the next tag (or end of string). If there is
// no such text (the first thing to parse is a tag or end of string,
// it should return NULL and leave s unchanged.

xml_node_t *parse_text()
{
  xml_node_t *node = NULL;
  char *text;
  char *s0;
  s0 = s;
  while (*s != '<' && *s != '\0')
  {
    s++;
  }
  int n = s - s0;
  if (n > 0)
  {
    text = calloc(n + 1, sizeof(char));
    strncpy(text, s0, n);
    node = calloc(1, sizeof(xml_node_t));
    node->type = TEXT;
    node->text = text;
  }
  return node;
}

// parse_element_name will parse an element name and return a newly allocated
// string with the name. If the name is empty, NULL is returned.

char *parse_element_name()
{
  char *text = NULL;
  char *s0;
  s0 = s;
  while (isalpha(*s))
  {
    s++;
  }
  int n = s - s0;
  if (n > 0)
  {
    text = calloc(n + 1, sizeof(char));
    strncpy(text, s0, n);
  }
  return text;
}

// parse_xml will parse a complete XML document string.

xml_node_t *parse_xml(char *in)
{
  xml_node_t *out;
  s = in;
  if (parse_whitespace() &&
      (out = parse_element()) &&
      parse_whitespace())
  {
    if (*s)
    {
      free_xml_tree(out);
      return NULL; // Junk after the XML element
    }
    else
    {
      return out; // OK.
    }
  }
  else
  {
    return NULL; // Parsing failed.
  }
}
