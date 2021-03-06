#include "../src/class.h"
#include "../src/class.c"
#include <math.h>
#include "tap.h"
#include <stdio.h>
#include <string.h>

int main(void) {
	dbl();
	test_long();
	fields();
	empty();
	test_field2str();
	return exit_status();
}	

/* Print i and j before calling ok(i == j, msg); */
void iok(int i, int j, char *msg) {
	printf("%d == %d \n", i, j, msg);
	ok(i == j, msg);
}

/* Print i and j before calling ok(i == j, msg); */
void lok(long i, long j, char *msg) {
	char *fmt_str = "%s - Comparison: '%ld' == '%ld'";
	int str_len = strlen(msg) + strlen(fmt_str) + 10 + 10 + 1;
	char *cmp_msg = malloc(sizeof(char) * str_len);
	snprintf(cmp_msg, str_len, fmt_str, msg, i, j);
	ok(i == j, cmp_msg);
}

/* Write a comparison string into msg and call ok(0 == strcmp(str1, str2), msg); */
void strok(char *str1, char *str2, char *msg) {
	char *fmt_str = "%s - Comparison: '%s' == '%s'";
	int str_len = strlen(msg) + strlen(fmt_str) + strlen(str1) + strlen(str2) + 1;
	char *cmp_msg = malloc(sizeof(char) * str_len);
	snprintf(cmp_msg, str_len, fmt_str, msg, str1, str2);
	ok(0 == strcmp(str1, str2), cmp_msg);
}

void dbl() {
	printh("Double");
	char *double_file = "./files/DoubleTest.class";
	Class *c = read_class_from_file_name(double_file);

	ok(c != NULL, "C is not NULL");
	ok((strcmp(c->file_name, double_file) == 0), "File name matches");
	ok(0 == c->minor_version, "Major version is 0 (1.7.0_10)");
	ok(51 == c->major_version, "Major version is 51 (1.7)");
	ok(1 == c->fields_count, "Fields count = 1");
	ok(31 == c->const_pool_count, "Constant pool count is 31"); // two for double type, plus # of items is 1 less than this member's value
	ok(0 == c->attributes_count, "Attributes count = 0");

	const Item *desc = get_item(c, c->fields[0].desc_idx);
	ok(desc != NULL, "Field descriptor Item is in the constant pool");
	ok('D' == desc->value.string.value[0], "Field type tag is D");

	ok(1 == c->fields[0].attrs_count, "Attribute count for field 0 is 1");

	const Item *attr_name = get_item(c, c->fields[0].attrs[0].name_idx);
	ok(strcmp("ConstantValue", attr_name->value.string.value) == 0, "First attribute in first field has name ConstantValue");

	// Constant pool content tests; could probably make a recursive function but this way is explicit & simpler
	Item *i = get_item(c, 1);
	ok(17 == i->value.ref.name_idx, " 1 = Methodref			   6");
	ok(6 == i->value.ref.class_idx, " 1 = Methodref          17         //  java/lang/Object.\"<init>\":()V");

	i = get_item(c, 2);
	ok(18 == i->value.ref.class_idx, " 2 = Fieldref           18        //  java/lang/System.out:Ljava/io/PrintStream;");
	ok(19 == i->value.ref.name_idx, " 2 = Fieldref           19        //  java/lang/System.out:Ljava/io/PrintStream;");

	i = get_item(c, 3);
	ok(20 == i->value.ref.class_idx, " 3 = String             20            //  Hello world1.0");

	i = get_item(c, 4);
	ok(21 == i->value.ref.class_idx, " 4 = Methodref         21        //  java/io/PrintStream.println:(Ljava/lang/String;)V");
	ok(22 == i->value.ref.name_idx, " 4 = Methodref          22        //  java/io/PrintStream.println:(Ljava/lang/String;)V");

	i = get_item(c, 5);
	iok(23, i->value.ref.class_idx, " 5 = Class              23            //  DoubleTest");

	i = get_item(c, 6);
	iok(24, i->value.ref.class_idx, " 6 = Class              24            //  java/lang/Object");

	i = get_item(c, 7);
	ok(0 == strcmp("d", i->value.string.value), " 7 = Utf8               d");

	i = get_item(c, 8);
	ok(0 == strcmp("D", i->value.string.value), " 8 = Utf8               D");

	i = get_item(c, 9);
	ok(0 == strcmp("ConstantValue", i->value.string.value), " 9 = Utf8               ConstantValue");

	i = get_item(c, 10);
	ok(1.0 == to_double(i->value.dbl), " 10 = Double             1.0d");

	i = get_item(c, 12);
	strok("<init>", i->value.string.value, " 12 = Utf8               <init>");

	i = get_item(c, 13);
	strok("()V", i->value.string.value, " 13 = Utf8               ()V");

	i = get_item(c, 14);
	strok("Code", i->value.string.value, " 14 = Utf8               Code");

	i = get_item(c, 15);
	strok("main", i->value.string.value, " 15 = Utf8               main");

	i = get_item(c, 16);
	strok("([Ljava/lang/String;)V", i->value.string.value, " 16 = Utf8               ([Ljava/lang/String;)V");

	i = get_item(c, 17);
	ok(12 == i->value.ref.class_idx, " 17 = NameAndType        12          \"<init>\":()V");
	ok(13 == i->value.ref.name_idx, " 17 = NameAndType        13          \"<init>\":()V");

	i = get_item(c, 18);
	ok(25 == i->value.ref.class_idx, " 18 = Class              25              java/lang/System");

	i = get_item(c, 19);
	ok(26 == i->value.ref.class_idx, " 19 = NameAndType        26          out:Ljava/io/PrintStream;");
	ok(27 == i->value.ref.name_idx, " 19 = NameAndType        27          out:Ljava/io/PrintStream;");

	i = get_item(c, 20);
	strok("Hello world1.0", i->value.string.value, " 20 = Utf8               Hello world1.0");

	i = get_item(c, 21);
	ok(28 == i->value.ref.class_idx, " 21 = Class              28              java/io/PrintStream");

	i = get_item(c, 22);
	ok(29 == i->value.ref.class_idx, " 22 = NameAndType        29          println:(Ljava/lang/String;)V");
	ok(30 == i->value.ref.name_idx, " 22 = NameAndType        30          println:(Ljava/lang/String;)V");

	i = get_item(c, 23);
	strok("DoubleTest", i->value.string.value, " 23 = Utf8               DoubleTest");

	i = get_item(c, 24);
	strok("java/lang/Object", i->value.string.value, " 24 = Utf8               java/lang/Object");

	i = get_item(c, 25);
	strok("java/lang/System", i->value.string.value, " 25 = Utf8               java/lang/System");

	i = get_item(c, 26);
	strok("out", i->value.string.value, " 26 = Utf8               out");

	i = get_item(c, 27);
	strok("Ljava/io/PrintStream;", i->value.string.value, " 27 = Utf8               Ljava/io/PrintStream;");

	i = get_item(c, 28);
	strok("java/io/PrintStream", i->value.string.value, " 28 = Utf8               java/io/PrintStream");

	i = get_item(c, 29);
	strok("println", i->value.string.value, " 29 = Utf8               println");

	i = get_item(c, 30);
	strok("(Ljava/lang/String;)V", i->value.string.value, " 30 = Utf8               (Ljava/lang/String;)V");

	const Method *method = c->methods;
	const Item *m1name = get_item(c, c->methods[0].name_idx);
	const Item *m2name = get_item(c, c->methods[1].name_idx);
	ok(c->methods_count == 2, "Methods count == 2, main() and constructor");
	ok(0 == strcmp("<init>", m1name->value.string.value), "First method's name is <init>");
	ok(0 == strcmp("main", m2name->value.string.value), "Second method's name is main");
	ok(1 == method->attrs_count, "init method attribute count is 1");

	// Test that attribute 1 (index 0) has the name "Code"
	const Item *m1attr1 = get_item(c, method->attrs[0].name_idx);
	ok(0 == strcmp("Code", m1attr1->value.string.value), "Attribute #1 of method #1 has name Code");
	ok(1 == c->methods[1].attrs_count, "main method attribute count is 1");
	free(c);
}

void empty() {
	printh("Empty");
	char *empty = "./files/Empty.class";
	Class *c = read_class_from_file_name(empty);

	ok(c != NULL, "C is not NULL");
	ok(0 == (strcmp(c->file_name, empty)), "File name matches");
	ok(0 == c->minor_version, "Major version is 0 (1.7.0_10)");
	ok(51 == c->major_version, "Major version is 51 (1.7)");
	ok(0 == c->fields_count, "Fields count = 0");
	ok(10 == c->const_pool_count, "Constant pool count is 10");
	ok(0 == c->attributes_count, "Attributes count = 0");

	free(c);
}

void test_long() {
	printh("Long");
	char *fname = "files/Fields.class";
	Class *c = read_class_from_file_name(fname);
	ok(NULL != c, "C is not NULL");
	ok(0 == (strcmp(c->file_name, fname)), "File name matches");
	ok(0 == c->minor_version, "Major version is 0 (1.7.0_10)");
	ok(51 == c->major_version, "Major version is 51 (1.7)");
	iok(1, c->fields_count, "Fields count = 1");
	iok(15, c->const_pool_count, "Constant pool count is 15");
	iok(0, c->attributes_count, "Attributes count = 0");
	
	iok(10, c->items[1].tag, "Item #1 tag byte is 10");
	strok(1, c->items[7].tag, "Item #7's tag byte is 1");
	strok("ConstantValue", c->items[7].value.string.value, "Item #7 is 'ConstantValue' UTF8");
	lok(1.0, to_long(c->items[8].value.lng), "Long constant pool item value is 1.0");
	strok("<init>", c->items[10].value.string.value, "Item #10 is '<init>' UTF8");
	free(c);
}

void fields() {
	printh("Fields");
	Class *c = read_class_from_file_name("files/Fields.class");
	ok(c != NULL, "C is not NULL");
	ok(c->fields_count == 7, "Fields count == 7");
	ok(c->methods_count == 2, "Methods count == 2, main() and constructor");
}

void test_field2str() {
	printh("field2str");
	ok("byte" == field2str('B'), "B == byte");
	ok("char" == field2str('C'), "C == char");
	ok("double" == field2str('D'), "D == double");
	ok("float" == field2str('F'), "F == float");
	ok("int" == field2str('I'), "I == int");
	ok("long" == field2str('J'), "J == long");
	ok("reference" == field2str('L'), "L == reference");
	ok("short" == field2str('S'), "S == short");
	ok("boolean" == field2str('Z'), "Z == boolean");
	ok("array" == field2str('['), "[ == array");
}

/* Print a pretty test header so we can distinguish results */
void printh(const char *test_name) {
	printf("#####################\n");
	printf("Test: %s\n", test_name);
	printf("#####################\n");
}
