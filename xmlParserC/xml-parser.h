typedef struct xml_node xml_node_t;
typedef struct node node_t;
typedef struct ioopm_stack ioopm_stack_t;
typedef struct xml_parse_tree xml_parse_tree_t;

// Nodes of the XML parse tree.
struct xml_node
{
  char *text; // Element name or text string
  bool type;  // Type of this node -- ELEMENT or TEXT
  bool status;
  xml_node_t *subnode; // Subtree list
  xml_node_t *next;    // Next sibling in subtree list of parent node
};

struct xml_parse_tree
{
  xml_node_t *root;
  xml_node_t *current;
  int size;
};

struct node
{
  xml_node_t *elem;
  node_t *next;
};

struct ioopm_stack
{
  node_t *top;
  int size;
};

#define ELEMENT false
#define TEXT true
#define CLOSED false
#define OPEN true

char *s;

char *parse_element_name();
xml_node_t *parse_element();
void free_xml_tree(xml_node_t *xml);
xml_node_t *parse_xml(char *in);
bool parse_whitespace();
bool maybe_parse_declaration();
xml_node_t *parse_text();
