// [try] method developed by Bil Corey
define try => {
    local(e) = pair(0='No error')
    local(gb) = givenBlock
    protect => {
        handle_error => { #e = pair(error_code = error_msg) }
        #gB()
    }
    #e->name != 0 ? fail(#e->name,#e->value)
}

define error_code_lspecNoSurroundingDescribe => 100
define error_msg_lspecNoSurroundingDescribe  => 'The [it] block is not encompassed in a [describe] block'



define it(label) => {
    fail_if(num_describes == 0, error_code_lspecNoSurroundingDescribe, error_msg_lspecNoSurroundingDescribe)
    blocks->insert(givenBlock)
}

define blocks => {
    not var(_blocks)? var(_blocks) = array
    return $_blocks
}




define describe(label) => {
    num_describes++
    handle => { num_describes-- }
    givenBlock()
    blocks->forEach => {#1->invoke}
}

define num_describes => {
    not var(_describes)? var(_describes) = 0
    return $_describes
}
define num_describes=(rhs::integer) => {
    not var(_describes)? var(_describes)
    $_describes = #rhs
}

/*



define describes => {
    not var(_describes)? var(_describes) = array
    return $_describes
}

define describe(label) => {
    
    stdoutnl(#label)
    givenBlock()
    its->forEach => {#1->invoke}
}

define it(label) => {
    stdoutnl('\t'+#label)
    its->insert(givenBlock)
}



describe("Foo") => {
    describe("Bar") => {
        local(test) = 0
    }
    it("does something") => {
        #test = 1
    }
}

stdoutnl('\t\t' + #test)
stdoutnl('\t\t' + #test)



describe => {1
    describe => {A
        describe => {B
            
        }
    }
    describe => {C
        describe => {D
            
        }
        describe => {E
            
        }
    }
    describe => {F
        
    }
}

1
array(A,C,F)
F
C
array(A,D,E)
E
D
A
array(B)
B
*/