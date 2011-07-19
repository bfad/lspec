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

/*
* Terminology:
*   test case  => each [expect] style statement
*   test       => each [it]  style statement
*   test group => each [describe] style statement
*   test suite => Compilation of all the test groups [lspec->suite]
*/

define error_code_lspecNoSurroundingDescribe => 100
define error_msg_lspecNoSurroundingDescribe  => 'The [it] block is not encompassed in a [describe] block'


define lspec_impl => type {
    data public suite  = array,
         public result = string,
         
         // Stats
         public numTests     = 0,
         public numTestCases = 0,
         public numSuccesses = 0,
         public numFailures  = 0,
         
         //Formatting options
         public printSuccesses = false,
         public indentor       = "    "
    
    public clearSuite => {
        //self = lspec_impl
        var(_lspec) = lspec_impl
        
        //TODO: RESTORE THE CURRENT FORMAT OPTIONS
    }
    
    public addTest(test::lspec_test) => {
        .suite->last->tests->insert(#test)
    }
    
    public output(tg::lspec_testGroup) => {
        .result != '' ? .result += '\n'
        .result += #tg->label
    }
    public output(test::lspec_test) => {
        .result += '\n' + .indentor + #test->label
    }
}

define lspec => {
    not var(_lspec)? var(_lspec) = lspec_impl
    return $_lspec
}

define lspec_testGroup => type {
    data public label::string,
         public tests = array
    
    public onCreate(label) => {
        .label = #label->asString
    }
}

define lspec_test => type {
    data public label::string,
         public block::capture
    
    public onCreate(label, gb::capture) => {
        .label = #label->asString
        .block = #gb
    }
}


define it(label) => {
    fail_if(not in_describe, error_code_lspecNoSurroundingDescribe, error_msg_lspecNoSurroundingDescribe)
    lspec->addTest(lspec_test(#label, givenBlock))
}


define describe(label) => {
    in_describe = true
    handle => { in_describe = false }
    
    local(me) = lspec_testGroup(#label)
    lspec->suite->insert(#me)
    lspec->output(#me)
    
    givenBlock()
    #me->tests->forEach => {
        local(cur_failures) = lspec->numFailures
        lspec->numTests++
        lspec->output(#1)
        
        #1->block->invoke
        
        lspec->numFailures == #cur_failures? 
            lspec->numSuccesses++
    }
}

define in_describe => {
    not var(_in_describe)? var(_in_describe) = false
    return $_in_describe
}
define in_describe=(rhs::boolean) => {
    not var(_in_describe)? var(_in_describe)
    $_in_describe = #rhs
}


define expect(test_case::boolean) => {
    lspec->numTestCases++
    not #test_case? lspec->numFailures++
}