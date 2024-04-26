#ifndef LISPINTERPRETER_CONS_H
#define LISPINTERPRETER_CONS_H

#include <memory>
#include <algorithm>
#include <cctype>
#include "Nodes.h"

class ConsNode : public Node {
private:
    std::shared_ptr<Node> car;
    std::shared_ptr<Node> cdr;

public:
    ConsNode(std::shared_ptr<Node> car, std::shared_ptr<Node> cdr) : car(std::move(car)), cdr(std::move(cdr)) {
        type = CONS;
    }

    std::string evaluate() override {
        std::string car_result = car->evaluate();
        std::string cdr_result = cdr->evaluate();

        cdr_result.pop_back(); // Remove the last space

        // Check if car_result is a list and should be treated as a sublist
        std::shared_ptr<ListNode> check = std::dynamic_pointer_cast<ListNode>(car);
        bool isCarSublist = (check->get_nodes_size() > 1);

        // Determine how to format the output based on the content of cdr_result
        if (cdr_result.empty()) {
            return "(" + car_result + ")";
        } else {
            if (isCarSublist) {
                car_result.pop_back(); // Remove the last space
                return "((" + car_result + ") " + cdr_result + ")";
            } else {
                return "(" + car_result + cdr_result + ")";
            }
        }
    }
};

#endif //LISPINTERPRETER_CONS_H
