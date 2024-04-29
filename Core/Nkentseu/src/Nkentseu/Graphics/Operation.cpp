//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/19/2024 at 12:17:26 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "Operation.h"

#include <Logger/Formatter.h>

namespace nkentseu {
    std::string Operation::ToString(Operation::Code operation)
    {
#define OPERATION_STR(op_i) if (Operation::op_i == operation) return #op_i

        OPERATION_STR(Add);
        OPERATION_STR(Multiply);
        OPERATION_STR(Substract);
        OPERATION_STR(Divide);
        OPERATION_STR(Modulo);
        OPERATION_STR(And);
        OPERATION_STR(Or);
        OPERATION_STR(Xor);
        return "NotDefine";
    }
    Operation::Code Operation::FromString(const std::string& operation)
    {
#define STR_OPERATION(op_i) if (std::string(#op_i) == operation) return Operation::op_i

        STR_OPERATION(Add);
        STR_OPERATION(Multiply);
        STR_OPERATION(Substract);
        STR_OPERATION(Divide);
        STR_OPERATION(Modulo);
        STR_OPERATION(And);
        STR_OPERATION(Or);
        STR_OPERATION(Xor);

        return Operation::NotDefine;
    }
}    // namespace nkentseu