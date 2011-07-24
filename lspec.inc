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
    
define error_code_lspecInvalidResultFormat => 102
define error_msg_lspecInvalidResultFormat  => "You have specified an invalid result format"


define lspec_impl => type {
    data public suite  = array,
         public result = string,
         
         // Flow status
         public inTestGroup  = false,
         public inTest       = false,
         public currentDepth = 0,
         
         // Stats
         public numTests     = 0,
         public numTestCases = 0,
         public numSuccesses = 0,
         public numFailures  = 0,
         public numPending   = 0,
         
         // Configuration Options
         private validFormats = (:'p', 'd'),
         public  options      = map(
                    'resultsFormat'  = "p",
                    'indentor'       = "    ",
                    'suppressOutput' = false,
         )
    
    public addTestGroup(tg::lspec_testGroup) => {
        if(.inTestGroup) => {
            .getCurrentTestGroup->groups->insert(#tg)
        else
            lspec->suite->insert(#tg)
            #tg->run
        }
    }
    public addTest(test::lspec_test) => {
        .getCurrentTestGroup->tests->insert(#test)
        .numTests++
    }
    
    private getCurrentTestGroup => {
        local(ret_val) = lspec->suite->last
        loop(.currentDepth) => {
            #ret_val = #ret_val->groups->last
        }
        return #ret_val
    }
    
    public output(tg::lspec_testGroup) => {
        .result != '' ? .result += '\n'
        .result += (.indentor * .currentDepth) + #tg->label
    }
    public output(test::lspec_test) => {
        .result += '\n' + (.indentor * .currentDepth) + .indentor + #test->label
    }
    
    // Options Setters & Getters
    public resultsFormat => .options->find('resultsFormat')
    public resultsFormat=(rhs::string) => {
        fail_if(not .validFormats->contains(#rhs), 
            error_code_lspecInvalidResultFormat, error_msg_lspecInvalidResultFormat)
        
        .options->find('resultsFormat') = #rhs
    }
    
    public indentor => .options->find('indentor')
    public indentor=(rhs::string) => { .options->find('indentor') = #rhs }
    
    public suppressOutput => .fromatting->find('suppressOutput')
    public suppressOutput=(rhs::boolean) => { .fromatting->find('suppressOutput') = #rhs }
}

define lspec => var(_lspec) || var(_lspec) := lspec_impl
// Could probably work around by throwing all the defaults into
// an onCreate method and then calling that, but would prefer not to
// as I'd have to remember to add every new data member to the oncreate
// even if it would be to just set it up with void
define lspec_clearSuite => {
    local(options) = lspec->options
    var(_lspec) = lspec_impl
    $_lspec->options = #options
}

define lspec_testGroup => type {
    data public label::string,
         public block::capture,
         public tests  = array,
         public groups = array
    
    public onCreate(label, gb::capture) => {
        .label = #label->asString
        .block = #gb
    }
    
    public numTests => .tests->size
    
    public run => {
        lspec->inTestGroup = true
        handle => { lspec->inTestGroup = false }
        
        lspec->output(self)
        .block->invoke
        .tests->forEach => {
            #1->run
        }
        .groups->forEach => {
            lspec->currentDepth++
            #1->run 
            lspec->currentDepth--
        }
    }
}

define lspec_test => type {
    data public label::string,
         public block::capture
    
    public onCreate(label, gb::capture) => {
        .label = #label->asString
        .block = #gb
    }
    
    public run => {
        local(cur_failures) = lspec->numFailures
        local(cur_cases)    = lspec->numTestCases

        lspec->output(self)

        lspec->inTest = true
        handle => { lspec->inTest = false }
        .block->invoke

        lspec->numTestCases == #cur_cases?
            lspec->numPending++
        | lspec->numFailures == #cur_failures? 
            lspec->numSuccesses++
    }
}


define describe(label) => {
    local(me) = lspec_testGroup(#label, givenBlock)
    lspec->addTestGroup(#me)
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