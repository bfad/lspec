/*
* Terminology:
*   test case  => each [expect] style statement
*   test       => each [it]  style statement
*   test group => each [describe] style statement
*   test suite => Compilation of all the test groups [lspec->suite]
*/

define error_code_lspecNotInTestGroup => 100
define error_msg_lspecNotInTestGroup  => 'The test block is not encompassed in a test group block'

define error_code_lspecNotInTest => 101
define error_msg_lspecNotInTest  => "The test case is not encompassed in a test block"


define lspec_impl => type {
    data public suite  = array,
         public result = string,
         
         // Flow status
         public inTestGroup = false,
         public inTest      = false,
         
         // Stats
         public numTests     = 0,
         public numTestCases = 0,
         public numSuccesses = 0,
         public numFailures  = 0,
         public numPending   = 0,
         
         //Formatting options
         public formatting = map(
                    'printSuccesses' = false,
                    'indentor'       = "    ",
                    'suppressOutput' = false,
         )
    
    public addTest(test::lspec_test) => {
        .suite->last->tests->insert(#test)
        .numTests++
    }
    
    public output(tg::lspec_testGroup) => {
        .result != '' ? .result += '\n'
        .result += #tg->label
    }
    public output(test::lspec_test) => {
        .result += '\n' + .indentor + #test->label
    }
    
    // Formatting Setters & Getters
    public printSuccesses => .formatting->find('printSuccesses')
    public printSuccesses=(rhs::boolean) => { .formatting->find('printSuccesses') = #rhs }
    
    public indentor => .formatting->find('indentor')
    public indentor=(rhs::string) => { .formatting->find('indentor') = #rhs }
    
    public suppressOutput => .fromatting->find('suppressOutput')
    public suppressOutput=(rhs::boolean) => { .fromatting->find('suppressOutput') = #rhs }
}

define lspec => var(_lspec) || var(_lspec) := lspec_impl
// Could probably work around by throwing all the defaults into
// an onCreate method and then calling that, but would prefer not to
// as I'd have to remember to add every new data member to the oncreate
// even if it would be to just set it up with void
define lspec_clearSuite => {
    local(formatting) = lspec->formatting
    var(_lspec) = lspec_impl
    $_lspec->formatting = #formatting
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


define describe(label) => {
    lspec->inTestGroup = true
    handle => { lspec->inTestGroup = false }
    
    local(me) = lspec_testGroup(#label)
    lspec->suite->insert(#me)
    lspec->output(#me)
    
    givenBlock()
    #me->tests->forEach => {
        local(cur_failures) = lspec->numFailures
        local(cur_cases)    = lspec->numTestCases
        
        lspec->output(#1)
        
        lspec->inTest = true
        handle => { lspec->inTest = false }
        #1->block->invoke
        
        lspec->numTestCases == #cur_cases?
            lspec->numPending++
        | lspec->numFailures == #cur_failures? 
            lspec->numSuccesses++
    }
}


define it(label) => {
    fail_if(not lspec->inTestGroup, error_code_lspecNotInTestGroup, error_msg_lspecNotInTestGroup)
    lspec->addTest(lspec_test(#label, givenBlock))
}


define expect(test_case::boolean) => {
    fail_if(not lspec->inTest, error_code_lspecNotInTest, error_msg_lspecNotInTest)
    lspec->numTestCases++
    not #test_case? lspec->numFailures++
}