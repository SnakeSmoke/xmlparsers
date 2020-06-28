#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "xml-parser.h"

int main()
{

  printf("Testing parse_element with empty tag...");
  s = "<>";
  xml_node_t *n2 = parse_element();
  if (n2)
  {
    printf(" ERROR returned a node but shouldn't\n");
  }
  else
  {
    printf(" OK\n");
  }

  printf("Testing parse_element with empty contents...");
  s = "<tag></tag>";
  xml_node_t *n3 = parse_element();
  if (!n3)
  {
    printf(" ERROR didn't return a node\n");
  }
  else if (strcmp(n3->text, "tag"))
  {
    printf(" ERROR returned wrong tag name\n");
  }
  else if (n3->type != ELEMENT)
  {
    printf(" ERROR returned wrong node type\n");
  }
  else if (n3->next)
  {
    printf(" ERROR returned a node with a sibling, which is wrong\n");
  }
  else if (n3->subnode)
  {
    printf(" ERROR returned a node with a subtree, which is wrong\n");
  }
  else if (*s)
  {
    printf(" ERROR string pointer should be positioned at the end of the string but isn't\n");
  }
  else
  {
    printf(" OK\n");
  }

  printf("Testing parse_element with mismatching tags...");
  s = "<tag></tagg>";
  xml_node_t *n3a = parse_element();
  if (n3a)
  {
    printf(" ERROR returned a node but shouldn't\n");
  }
  else
  {
    printf(" OK\n");
  }

  printf("Testing parse_element with text contents...");
  s = "<tag>Some text</tag>";
  xml_node_t *n4 = parse_element();
  if (!n4)
  {
    printf(" ERROR didn't return a node\n");
  }
  else if (strcmp(n4->text, "tag"))
  {
    printf(" ERROR returned wrong tag name\n");
  }
  else if (n4->type != ELEMENT)
  {
    printf(" ERROR returned wrong node type\n");
  }
  else if (n4->next)
  {
    printf(" ERROR returned a node with a sibling, which is wrong\n");
  }
  else if (!n4->subnode)
  {
    printf(" ERROR returned a node without a subtree, which is wrong\n");
  }
  else if (strcmp(n4->subnode->text, "Some text") || n4->subnode->type != TEXT)
  {
    printf(" ERROR subtree is wrong\n");
  }
  else if (*s)
  {
    printf(" ERROR string pointer should be positioned at the end of the string\n but isn't\n");
  }
  else
  {
    printf(" OK\n");
  }

  printf("Testing example '<p>Lite <it>olika</it> text</p>' ...");
  xml_node_t *n5 = parse_xml("<p>Lite <it>olika</it>text</p>\n");
  xml_node_t *n51 = n5->subnode;
  xml_node_t *n52 = n51->next;
  xml_node_t *n521 = n52->subnode;
  xml_node_t *n53 = n52->next;

  printf("\nn5 ->%s, status -> %d\n", n5->text, n5->status);
  printf("n51 ->%s, status -> %d\n", n51->text, n51->status);
  printf("n52 ->%s, status -> %d\n", n52->text, n52->status);
  printf("n521 ->%s, status -> %d\n", n521->text, n521->status);
  printf("n53 ->%s, status -> %d\n", n53->text, n53->status);
  if (!n5)
  {
    printf(" ERROR didn't return a tree\n");
  }
  else if (!n51)
  {
    printf(" ERROR no subtrees\n");
  }
  else if (!n52)
  {
    printf(" ERROR only one subtree -- no sibling to first subtree node\n");
  }
  else if (!n53)
  {
    printf(" ERROR only two subtrees -- no sibling to second subtree node\n");
  }
  else if (n53->next)
  {
    printf(" ERROR more than three subtrees -- there is a sibling to the third subtree node\n");
  }
  else if (!n521)
  {
    printf(" ERROR no subtree of 'it' element node\n");
  }
  else if (strcmp(n5->text, "p"))
  {
    printf(" ERROR root node is wrong\n");
  }
  else if (strcmp(n51->text, "Lite "))
  {
    printf(" ERROR first subtree is wrong\n");
  }
  else if (strcmp(n52->text, "it"))
  {
    printf(" ERROR second subtree is wrong\n");
  }
  else if (strcmp(n53->text, "text"))
  {

    printf(" ERROR third subtree is wrong got -> %s\n", n53->text);
  }
  else if (strcmp(n521->text, "olika"))
  {
    printf(" ERROR subtree of 'it' element node is wrong\n");
  }
  else
  {
    printf(" OK, I think, but I've only made spot checks.\n");
  };

  printf("Testing free_xml_tree... ");
  free_xml_tree(n5);
  free_xml_tree(n4);
  free_xml_tree(n3a);
  free_xml_tree(n3);
  free_xml_tree(n2);

  printf("OK, in the sense that it didn't crash or loop. You have to do 'make memtest' to check that all memory is properly freed.\n");
  return 0;
}
