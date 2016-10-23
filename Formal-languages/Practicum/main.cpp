/**
 * Automaton
 * @author Ilya I. Romanenko
 * @since 24.10.16
 **/
#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <queue>
#include <stack>

using namespace std;

const char EPSILON = '1';

class Automaton {
    struct node {
        map<char, vector<node *> > go;
    };
    node *begin, *end;

    Automaton() {
        begin = new node();
        end = new node();
    }

    void addToBeginNode(char alpha, node *subtree) {
        begin->go[alpha].push_back(subtree);
    }

public:

    Automaton(char alpha) {
        begin = new node();
        end = new node();
        addToBeginNode(alpha, end);
    }

    Automaton operator+(Automaton ob) {
        Automaton newAutomaton;
        newAutomaton.addToBeginNode(EPSILON, begin);
        newAutomaton.addToBeginNode(EPSILON, ob.begin);
        end->go[EPSILON].push_back(newAutomaton.end);
        ob.end->go[EPSILON].push_back(newAutomaton.end);
        return newAutomaton;
    }

    Automaton operator*(Automaton ob) {
        Automaton newAutomaton;
        newAutomaton.addToBeginNode(EPSILON, begin);
        end->go[EPSILON].push_back(ob.begin);
        ob.end->go[EPSILON].push_back(newAutomaton.end);
        return newAutomaton;
    }

    Automaton operator^(bool) {
        Automaton newAutomaton;
        newAutomaton.addToBeginNode(EPSILON, begin);
        end->go[EPSILON].push_back(begin);
        end->go[EPSILON].push_back(newAutomaton.end);
        return newAutomaton;
    }

    void addAllSuffix() {
        set<node *> used;
        queue<node *> q;
        q.push(begin);
        used.insert(begin);

        while (!q.empty()) {
            node *root = q.front();
            q.pop();
            addToBeginNode(EPSILON, root);

            for (auto goNodesVector : root->go) {
                for (auto nodeToGo : goNodesVector.second) {
                    if (used.find(nodeToGo) == used.end()) {
                        q.push(nodeToGo);
                        used.insert(nodeToGo);
                    }
                }
            }
        }
    }

    bool hasAccept(const string &s) {
        set<node *> oldState, newState, used;
        newState.insert(begin);

        string toFind = s + '1';
        for (char alpha : toFind) {
            swap(newState, oldState);
            newState.clear();
            used.clear();

            queue<node *> stateNodes;
            for (auto oldNode : oldState) {
                stateNodes.push(oldNode);
                used.insert(oldNode);
            }

            while (!stateNodes.empty()) {
                node *current = stateNodes.front();
                stateNodes.pop();

                for (auto nodeWithEpsilonMove : current->go[EPSILON]) {
                    if (used.find(nodeWithEpsilonMove) == used.end()) {
                        stateNodes.push(nodeWithEpsilonMove);
                        used.insert(nodeWithEpsilonMove);
                    }
                }

                for (auto nodeWithAlphaMove : current->go[alpha]) {
                    if (newState.find(nodeWithAlphaMove) == newState.end()) {
                        newState.insert(nodeWithAlphaMove);
                    }
                }

            }
        }

        return newState.find(end) != newState.end();
    }
};

int main() {
    string s;
    char x;
    unsigned int k;
    cin >> s >> x >> k;

    string toFind(k, x);

    stack<Automaton> automatons;
    for (size_t i = 0; i < s.length(); i++) {
        switch (s[i]) {
            case '+': {
                if (automatons.size() < 2)
                    throw logic_error("Not correct notation");

                Automaton first = automatons.top();
                automatons.pop();
                Automaton second = automatons.top();
                automatons.pop();

                automatons.push(first + second);
                break;
            }
            case '.': {
                if (automatons.size() < 2)
                    throw logic_error("Not correct notation");

                Automaton first = automatons.top();
                automatons.pop();
                Automaton second = automatons.top();
                automatons.pop();

                automatons.push(second * first);
                break;
            }
            case '*': {
                if (automatons.size() < 1)
                    throw logic_error("Not correct notation");

                Automaton first = automatons.top();
                automatons.pop();
                automatons.push(first ^ true);
                break;
            }
            default: {
                automatons.push(Automaton(s[i]));
            }
        }
    }
    if (automatons.size() != 1)
        throw logic_error("Not correct notation");


    Automaton result = automatons.top();

    result.addAllSuffix();
    if (result.hasAccept(toFind)) {
        cout << "YES" << endl;
    } else {
        cout << "NO" << endl;
    }

    return 0;
}
