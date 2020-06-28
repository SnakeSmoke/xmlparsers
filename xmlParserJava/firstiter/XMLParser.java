import java.util.LinkedList;



class XML_text_node extends XML_node {
	String text;

	public XML_text_node(String text) {
		this.text = text;
	}
} 

// XML_element_node represents an element

class XML_element_node extends XML_node {
	String name;
	boolean open;

	public XML_element_node(String name) {
		this.name = name;
		this.open = true;
	}

	public XML_element_node() {
		this.name = "root";
	}
} 

class XML_parser {

	// parse_element parses an element beginning with a start tag, followed by
	// zero or more text parts and/or sub-elements, and finally an end tag.
	// It returns an ELEMENT node with subtrees for all text and sub-elements.
	// In case of a parse error or if the start and end tags don't match,
	// NULL is returned.

	XML_element_node parse_element() {

		XML_element_node root = new XML_element_node();

		// System.out.println("\nString to parse: " + s);

		if (s.isEmpty() || s.contains("<>")) {
			return null;
		}

		LinkedList<XML_element_node> visited_elements = new LinkedList<XML_element_node>();

		while (!endofstring) {
			if (currchar == '<') {
				// System.out.println("Found '<' and starting to parse!");
				nextchar();
				// System.out.println("currentChar is this -> " + currchar);
				if (currchar == '/') {
					// System.out.println("Found '/' and starting to parse!");
					nextchar();
					// System.out.println("currentChar is this -> " + currchar);
					boolean element_is_in_list = parse_and_check_closing_tag(visited_elements);
					if (!element_is_in_list) {
						return null;
					}
				} else {
					XML_element_node n = new XML_element_node(parse_element_name());
					// System.out.println("Found this -> " + n.name);
					if (visited_elements.size() == 0) {
						root.subnode = n;
						if (n.name.isEmpty()) {
							return null;
						}
					} else {
						if (visited_elements.getLast().open) {
							if (visited_elements.getLast().subnode != null) {
								visited_elements.getLast().subnode.next = n;

							} else {
								visited_elements.getLast().subnode = n;
							}
						} else {
							visited_elements.getLast().next = n;
						}
					}
					visited_elements.add(n);

				}

			}
			if (currchar == '>') {
				// print_element_pointer();
				// System.out.println("currentChar before nextchar is this -> " + currchar);
				nextchar();
				// System.out.println("currentChar is this -> " + currchar);
				if (Character.isLetter(currchar) || Character.isWhitespace(currchar) && currchar != '\n') {
					XML_text_node t = parse_text();
					// System.out.println("t -> " + t.text);
					// System.out.println("************ " + visited_elements.getLast().name + " is
					// open?: "
					// + visited_elements.getLast().open);
					if (visited_elements.getLast().open) {
						// System.out.println("inserting " + t.text + " as a subnode of " +
						// visited_elements.getLast().name);
						// System.out.println("last visited -> " + visited_elements.getLast().name);
						visited_elements.getLast().subnode = t;
						// System.out.println("inserted t as subnode to " +
						// visited_elements.getLast().name);
						// System.out.println("checking subnode");
						// XML_text_node test = (XML_text_node) visited_elements.getLast().subnode;
						// System.out.println(test.text);
					} else {
						// System.out.println("inserting " + t.text + " as a next of " +
						// visited_elements.getLast().name);
						visited_elements.getLast().next = t;
						// XML_text_node test = (XML_text_node) visited_elements.getLast().next;
						// System.out.println("Testing if it worked! " + test.text);
					}
				} else {
					prevchar();
				}
			}
			// System.out.println("last currchar -> " + currchar);
			if (currchar == '<') {
				prevchar();
			}
			nextchar();
		}

		return (XML_element_node) root.subnode;
	}

	// parse_whitespace will simply skip over any whitespace. Always succeeds.
	boolean parse_whitespace() {
		while (!endofstring && (currchar == ' ' || currchar == '\t' || currchar == '\n')) {
			nextchar();
		}
		return true;
	}

	boolean parse_and_check_closing_tag(LinkedList<XML_element_node> l) {

		String closing_tag = parse_element_name();

		for (XML_element_node node : l) {
			if (node.name.equals(closing_tag)) {
				node.open = false;
				return true;
			}
		}
		return false;
	}

	void print_element_pointer() {

		System.out.println("\n________________");
		System.out.println("string -> " + s);
		System.out.println("currchar -> " + currchar);
		System.out.println("index -> " + p);
		System.out.println("________________");
	}

	// parse_element_name will parse an element name and return a
	// string with the name. Always succeeds unless the name is empty.
	String parse_element_name() {
		String text = "";
		while (!endofstring && Character.isLetter(currchar)) {
			text = text + currchar;
			nextchar();
		}
		return text;
	}

	// parse_text parses text inside elements. It returns a TEXT node with
	// text up to but not including the next tag (or end of string). If there is
	// no such text (the first thing to parse is a tag or end of string,
	// it returns null and leaves s unchanged.

	XML_text_node parse_text() {
		XML_text_node node = null;

		String text = "";
		while (!endofstring && currchar != '<') {
			text = text + currchar;
			nextchar();
		}
		if (!text.equals("")) {
			node = new XML_text_node(text);
		}
		return node;
	}

	// parse_xml will parse a complete XML document string.
	XML_node parse_xml() {
		XML_node out;
		if (parse_whitespace() && (out = parse_element()) != null && parse_whitespace()) {
			if (!endofstring) {
				return null; // Junk after the XML element
			} else {
				return out; // OK.
			}
		} else {
			return null; // Parsing failed.
		}
	}

	String s; // Holds the string being parsed
	int p; // Current position in the string being parsed
	char currchar; // Current character in the string being parsed
	boolean endofstring; // The end of the string being parsed is reached

	// nextchar advances to the next character to be parsed
	void nextchar() {
		p++;
		endofstring = p >= s.length();
		if (!endofstring) {
			currchar = s.charAt(p);
		}
	}

	// nextchar backs up to the previous character in the string to be parsed
	void prevchar() {
		p = p - 2;
		nextchar();
	}

	// Creates a new parser object with a string to be parsed
	XML_parser(String s) {
		this.s = s;
		this.p = -1;
		nextchar();
	}

}

class XML_node {
	XML_node subnode;
	XML_node next;
}

public class YourCode {
}
