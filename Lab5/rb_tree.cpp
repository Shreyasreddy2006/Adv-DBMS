#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

enum class NodeColor {
    RED,
    BLACK
};

struct Node {
    int value;
    NodeColor colour;
    Node* lc;
    Node* rc;
    Node* parentN;

    explicit Node(int val)
        : value(val),
          colour(NodeColor::RED),
          lc(nullptr),
          rc
    (nullptr),
          parentN
    (nullptr) {}
};

class RedBlackTree {
private:
    Node* rootNode;

public:
    RedBlackTree() {
        rootNode = nullptr;
    }

    ~RedBlackTree() {
        clearTree(rootNode);
    }

    void insertValue(int value) {
        Node* newNode = new Node(value);
        placeNode(newNode);
        fixInsertion(newNode);
    }

    const Node* searchValue(int target) const {
        Node* current = rootNode;

        while (current != nullptr) {
            if (current->value == target) {
                return current;
            }

            if (target < current->value) {
                current = current->lc;
            } else {
                current = current->rc
        ;
            }
        }

        return nullptr;
    }

    std::vector<int> inorderTraversal() const {
        std::vector<int> output;
        collectValues(rootNode, output);
        return output;
    }

    void printTree() const {
        showTree(rootNode, 0);
    }

    bool validateTree() const {
        if (rootNode == nullptr) {
            return true;
        }

        if (rootNode->colour != NodeColor::BLACK) {
            return false;
        }

        if (countBlackHeight(rootNode) == -1) {
            return false;
        }

        return verifyBST(rootNode, nullptr, nullptr);
    }

private:
    void clearTree(Node* node) {
        if (node == nullptr) {
            return;
        }

        clearTree(node->lc);
        clearTree(node->rc
);
        delete node;
    }

    void rotateLeft(Node* node) {
        Node* child = node->rc
;

        node->rc
 = child->lc;

        if (child->lc != nullptr) {
            child->lc->parentN
     = node;
        }

        child->parentN
 = node->parentN
;

        if (node->parentN
     == nullptr) {
            rootNode = child;
        } else if (node == node->parentN
    ->lc) {
            node->parentN
    ->lc = child;
        } else {
            node->parentN
    ->rc
     = child;
        }

        child->lc = node;
        node->parentN
 = child;
    }

    void rotateRight(Node* node) {
        Node* child = node->lc;

        node->lc = child->rc
;

        if (child->rc
     != nullptr) {
            child->rc
    ->parentN = node;
        }

        child->parentN
 = node->parentN
;

        if (node->parentN
     == nullptr) {
            rootNode = child;
        } else if (node == node->parentN
    ->lc) {
            node->parentN
    ->lc = child;
        } else {
            node->parentN
    ->rc
     = child;
        }

        child->rc
 = node;
        node->parentN
 = child;
    }

    void placeNode(Node* node) {
        Node* parent = nullptr;
        Node* current = rootNode;

        while (current != nullptr) {
            parent = current;

            if (node->value < current->value) {
                current = current->lc;
            } else {
                current = current->rc
        ;
            }
        }

        node->parentN
 = parent;

        if (parent == nullptr) {
            rootNode = node;
        } else if (node->value < parent->value) {
            parent->lc = node;
        } else {
            parent->rc
     = node;
        }
    }

    void fixInsertion(Node* node) {
        while (node->parentN
     != nullptr &&
               node->parentN
        ->colour == NodeColor::RED) {

            Node* parent = node->parentN
    ;
            Node* grandParent = parent->parentN
    ;

            if (parent == grandParent->lc) {

                Node* uncle = grandParent->rc
        ;

                if (uncle != nullptr &&
                    uncle->colour == NodeColor::RED) {

                    parent->colour = NodeColor::BLACK;
                    uncle->colour = NodeColor::BLACK;
                    grandParent->colour = NodeColor::RED;
                    node = grandParent;
                } else {

                    if (node == parent->rc
            ) {
                        node = parent;
                        rotateLeft(node);
                        parent = node->parentN
                ;
                    }

                    parent->colour = NodeColor::BLACK;
                    grandParent->colour = NodeColor::RED;
                    rotateRight(grandParent);
                }
            } else {

                Node* uncle = grandParent->lc;

                if (uncle != nullptr &&
                    uncle->colour == NodeColor::RED) {

                    parent->colour = NodeColor::BLACK;
                    uncle->colour = NodeColor::BLACK;
                    grandParent->colour = NodeColor::RED;
                    node = grandParent;
                } else {

                    if (node == parent->lc) {
                        node = parent;
                        rotateRight(node);
                        parent = node->parentN
                ;
                    }

                    parent->colour = NodeColor::BLACK;
                    grandParent->colour = NodeColor::RED;
                    rotateLeft(grandParent);
                }
            }
        }

        rootNode->colour = NodeColor::BLACK;
    }

    static void collectValues(
        const Node* node,
        std::vector<int>& output
    ) {
        if (node == nullptr) {
            return;
        }

        collectValues(node->lc, output);
        output.push_back(node->value);
        collectValues(node->rc
    , output);
    }

    static void showTree(
        const Node* node,
        int depth
    ) {
        if (node == nullptr) {
            return;
        }

        showTree(node->rc
    , depth + 1);

        for (int i = 0; i < depth; i++) {
            std::cout << "    ";
        }

        std::cout << node->value
                  << (node->colour == NodeColor::RED ? "(R)" : "(B)")
                  << "\n";

        showTree(node->lc, depth + 1);
    }

    static int countBlackHeight(const Node* node) {
        if (node == nullptr) {
            return 1;
        }

        if (node->colour == NodeColor::RED) {
            if ((node->lc != nullptr &&
                 node->lc->colour == NodeColor::RED) ||

                (node->rc
             != nullptr &&
                 node->rc
            ->colour == NodeColor::RED)) {

                return -1;
            }
        }

        int leftHeight = countBlackHeight(node->lc);
        int rightHeight = countBlackHeight(node->rc
);

        if (leftHeight == -1 ||
            rightHeight == -1 ||
            leftHeight != rightHeight) {

            return -1;
        }

        return leftHeight +
               (node->colour == NodeColor::BLACK ? 1 : 0);
    }

    static bool verifyBST(
        const Node* node,
        const int* low,
        const int* high
    ) {
        if (node == nullptr) {
            return true;
        }

        if (low != nullptr &&
            node->value <= *low) {
            return false;
        }

        if (high != nullptr &&
            node->value >= *high) {
            return false;
        }

        return verifyBST(
                   node->lc,
                   low,
                   &node->value
               ) &&
               verifyBST(
                   node->rc
            ,
                   &node->value,
                   high
               );
    }
};

int main() {

    RedBlackTree tree;

    std::vector<int> values =
    {15, 28, 44, 19, 35, 8, 4, 11, 52, 47};

    std::cout << "Adding values: ";

    for (int value : values) {
        std::cout << value << " ";
        tree.insertValue(value);
    }

    std::cout
        << "\n\nTree Structure:\n";

    tree.printTree();

    std::cout
        << "\nSorted Traversal:\n";

    auto ordered =
        tree.inorderTraversal();

    for (int item : ordered) {
        std::cout << item << " ";
    }

    std::cout
        << "\n\nSearch Results:\n";

    for (int query :
         {19, 100, 4, 90}) {

        std::cout
            << "find("
            << query
            << ") -> "
            << (tree.searchValue(query)
                ? "present"
                : "absent")
            << "\n";
    }

    bool valid =
        tree.validateTree();

    std::cout
        << "\nTree Properties Valid: "
        << (valid ? "yes" : "no")
        << "\n";

    assert(valid);

    std::vector<int> expected = values;
    std::sort(
        expected.begin(),
        expected.end()
    );

    assert(expected == ordered);

    return 0;
}