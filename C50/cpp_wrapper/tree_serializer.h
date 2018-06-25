#ifndef __TREE_SERIALIZER_H__
#define __TREE_SERIALIZER_H__

/**
 * Returns a JSON serialization of a C5 decision tree. The object underlying the
 * serialization, say named "Node" has to contain the following attributes:
 *
 *    - string attribute
 *    - int cut
 *    - string classification
 *    - Node left
 *    - Node right
 *    - bool isLeaf
 *
 * This method can only handle decision trees with continous attributes.
 *
 * @param t The tree to serialize
 *
 * @returns a JSON serialization of a C5 decision tree.
 */
char * serialize_to_JSON (Tree t);

#endif
