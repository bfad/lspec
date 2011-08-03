/*
* Terminology:
*   test case  => each [expect] style statement
*   test       => each [it]  style statement
*   test group => each [describe] style statement
*   test suite => Compilation of all the test groups [lspec->suite]
*/
define error_code_missingGivenBlock => 10
define error_msg_missingGivenBlock  => 'This method requires a block'

define error_code_lspecNotInTestGroup => 100
define error_msg_lspecNotInTestGroup  => 'The test block is not encompassed in a test group block'

define error_code_lspecNotInTest => 101
define error_msg_lspecNotInTest  => "The test case is not encompassed in a test block"
    
define error_code_lspecTestGroupInTest => 102
define error_msg_lspecTestGroupInTest  => "The test group is in a test block"
    
define error_code_lspecInvalidResultFormat => 103
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
    
    public getCurrentTest => {
        return .getCurrentTestGroup->tests->get(.getCurrentTestGroup->currentTest)
    }
    
    public output(value::string) => {
        .result += #value
        not .suppressOutput? stdout(#value)
    }
    public output(tg::lspec_testGroup) => {
        local(out) = (.indentor * .currentDepth) + #tg->label
        .result != '' ? #out = '\n' + #out
        .output(#out)
    }
    public output(test::lspec_test) => {
        local(out) = '\n' + (.indentor * .currentDepth) + .indentor + #test->label
        .output(#out)
    }
    
    public outputStatus(status::string) => {
        #status != 'fail'? return
        .output(' (FAILED - ' + .numFailures + ')')
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
    
    public suppressOutput => .options->find('suppressOutput')
    public suppressOutput=(rhs::boolean) => { .options->find('suppressOutput') = #rhs }
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
         public groups = array,
         // Status
         public currentTest = 0
    
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
        .currentTest = 0
        .tests->forEach => {
            .currentTest++
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
         public block::capture,
         public status::string
    
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
        local(gb_error) = pair(0='No error')
        protect => {
            handle_error => { #gb_error = pair(error_code=error_msg) }
            .block->invoke 
        }

        if(#gb_error->first != 0) => {
            failure_clear
            error_reset
            lspec->numFailures++
            .status = 'fail'
        else(lspec->numTestCases == #cur_cases)
            lspec->numPending++
            .status = 'pending'
        else(not .status)
            lspec->numSuccesses++
            .status = 'success'
        else
            lspec->numFailures++
        }
        
        lspec->outputStatus(.status)
    }
}


define describe(label) => {
    fail_if(lspec->inTest, 
        error_code_lspecTestGroupInTest, error_msg_lspecTestGroupInTest)
    local(me) = lspec_testGroup(#label, givenBlock)
    lspec->addTestGroup(#me)
}


define it(label) => {
    fail_if(not lspec->inTestGroup, error_code_lspecNotInTestGroup, error_msg_lspecNotInTestGroup)
    lspec->addTest(lspec_test(#label, givenBlock))
}


define expect => type {
    public onCreate => {
        fail_if(not lspec->inTest, error_code_lspecNotInTest, error_msg_lspecNotInTest)
        lspec->numTestCases++
    }
    public onCreate(test_case::boolean) => {
        .onCreate
        if(not #test_case) => {
            lspec->getCurrentTest->status = 'fail'
        }
    }
}

define expect->error(code::integer, msg::string) => {
    fail_if(not givenBlock,
        error_code_missingGivenBlock, error_msg_missingGivenBlock)
    
    local(my_error) = pair(error_code_noerror=error_msg_noerror)
    protect => {
        handle_error => { #my_error = pair(error_code=error_msg) }
        givenBlock()
    }
    
    if(#my_error->first != #code or #my_error->second != #msg) => {
        lspec->getCurrentTest->status = 'fail'
        fail(#my_error->first,#my_error->second)
    else
        failure_clear
        error_reset
    }
}

define expect->stdout(expected::string) => {
    fail_if(not givenBlock,
        error_code_missingGivenBlock, error_msg_missingGivenBlock)
    
    // save stdout using ->dup
    // create a local conduit
    // close real stout, reopen it on our write pipe using dup2 system call
    // make a file object here which we read stout from
    local(save_stdout) = file_stdout->fd->dup
    local(readFd, writeFd) = io_file_pipe
    #writeFd->dup2(file_stdout->fd)
    local(read_test) = file(#readFd)
    
    handle => {
        // set stout back using our save
        #save_stdout->dup2(file_stdout->fd)
    }
    
    givenBlock()
    
    // read test data
    local(did_read) = #read_test->readString
    
    #did_read != #expected? 
        lspec->getCurrentTest->status = 'fail'
}