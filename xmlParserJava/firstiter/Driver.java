public class Driver {
	public static void main(String[] args) {

		XML_parser p;

		System.out.printf("Testing parse_text with empty string...");
		p = new XML_parser("");
		if (p.parse_text() != null) {
			System.out.printf(" ERROR returned a node but shouldn't\n");
		} else {
			System.out.printf(" OK\n");
		}

		System.out.printf("Testing parse_text with empty text before tag...");
		p = new XML_parser("<tag>");
		if (p.parse_text() != null) {
			System.out.printf(" ERROR returned a node but shouldn't\n");
		} else if (p.p != 0) {
			System.out.printf(" ERROR string pointer should be unchanged but isn't\n");
		} else {
			System.out.printf(" OK\n");
		}

		System.out.printf("Testing parse_text with some text before tag...");
		p = new XML_parser(" some text <tag>abc");
		XML_text_node n1 = p.parse_text();
		if (n1 == null) {
			System.out.printf(" ERROR didn't return a node\n");
		} else if (n1.text == " some text ") {
			System.out.printf(" ERROR returned wrong text\n");
		} else if (n1.next != null) {
			System.out.printf(" ERROR returned a node with a sibling, which is wrong\n");
		} else if (n1.subnode != null) {
			System.out.printf(" ERROR returned a node with a subtree, which is wrong\n");
		} else if (p.p != 11) {
			System.out.printf(" ERROR string pointer should be positioned at the beginning of the tag but isn't\n");
		} else {
			System.out.printf(" OK\n");
		}

		System.out.printf("Testing parse_element with empty tag...");
		p = new XML_parser("<>");
		XML_element_node n2 = p.parse_element();
		if (n2 != null) {
			System.out.printf(" ERROR returned a node but shouldn't\n");
		} else {
			System.out.printf(" OK\n");
		}

		System.out.printf("Testing parse_element with empty contents...");
		p = new XML_parser("<tag></tag>");
		XML_element_node n3 = p.parse_element();
		if (n3 == null) {
			System.out.printf(" ERROR didn't return a node\n");
		} else if (!n3.name.equals("tag")) {
			System.out.printf(" ERROR returned wrong tag name\n");
		} else if (n3.next != null) {
			System.out.printf(" ERROR returned a node with a sibling, which is wrong\n");
		} else if (n3.subnode != null) {
			System.out.printf(" ERROR returned a node with a subtree, which is wrong\n");
		} else if (!p.endofstring) {
			System.out.printf(" ERROR string pointer should be positioned at the end of the string but isn't\n");
		} else {
			System.out.printf(" OK\n");
		}

		System.out.printf("Testing parse_element with mismatching tags...");
		p = new XML_parser("<tag></tagg>");
		XML_element_node n3a = p.parse_element();
		if (n3a != null) {
			System.out.printf(" ERROR returned a node but shouldn't\n");
		} else {
			System.out.printf(" OK\n");
		}

		System.out.printf("Testing parse_element with text contents...");
		p = new XML_parser("<tag>Some text</tag>");
		XML_element_node n4 = p.parse_element();
		if (n4 == null) {
			System.out.printf(" ERROR didn't return a node\n");
		} else if (!n4.name.equals("tag")) {
			System.out.printf(" ERROR returned wrong tag name\n");
		} else if (n4.next != null) {
			System.out.printf(" ERROR returned a node with a sibling, which is wrong\n");
		} else if (n4.subnode == null) {
			System.out.printf(" ERROR returned a node without a subtree, which is wrong\n");
		} else if (!(n4.subnode instanceof XML_text_node) || !((XML_text_node) n4.subnode).text.equals("Some text")) {
			System.out.printf(" ERROR subtree is wrong\n");
		} else if (!p.endofstring) {
			System.out.printf(" ERROR string pointer should be positioned at the end of the string\n but isn't\n");
		} else {
			System.out.printf(" OK\n");
		}

		System.out.printf("Testing example '<p>Lite <it>olika</it> text</p>' ...");
		p = new XML_parser("<p>Lite <it>olika</it> text</p>\n");
		XML_node n5 = p.parse_xml();

		if (n5 == null) {
			System.out.printf(" ERROR didn't return a tree\n");
		} else {
			XML_node n51 = n5.subnode;
			if (n51 == null) {
				System.out.printf(" ERROR no subtrees\n");
			} else {
				XML_node n52 = n51.next;
				if (n52 == null) {
					System.out.printf(" ERROR only one subtree -- no sibling to first subtree node\n");
				} else {
					XML_node n521 = n52.subnode;
					XML_node n53 = n52.next;
					if (n53 == null) {
						System.out.printf(" ERROR only two subtrees -- no sibling to second subtree node\n");
					} else if (n53.next != null) {
						System.out.printf(
								" ERROR more than three subtrees -- there is a sibling to the third subtree node\n");
					} else if (n521 == null) {
						System.out.printf(" ERROR no subtree of 'it' element node\n");
					} else if (!(n5 instanceof XML_element_node) || !((XML_element_node) n5).name.equals("p")) {
						System.out.printf(" ERROR root node is wrong\n");
					} else if (!(n51 instanceof XML_text_node) || !((XML_text_node) n51).text.equals("Lite ")) {
						System.out.printf(" ERROR first subtree is wrong\n");
					} else if (!(n52 instanceof XML_element_node) || !((XML_element_node) n52).name.equals("it")) {
						System.out.printf(" ERROR second subtree is wrong\n");
					} else if (!(n53 instanceof XML_text_node) || !((XML_text_node) n53).text.equals(" text")) {
						System.out.printf(" ERROR third subtree is wrong\n");
					} else if (!(n521 instanceof XML_text_node) || !((XML_text_node) n521).text.equals("olika")) {
						System.out.printf(" ERROR subtree of 'it' element node is wrong\n");
					} else {
						System.out.printf(" OK, I think, but I've only made spot checks.\n");
					}
				}
			}
		}
	}

}
