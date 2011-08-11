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

define error_code_lspecStopInTestGroup => 103
define error_msg_lspecStopInTestGroup  => "Can not stop while in a test group"
    
define error_code_lspecInvalidResultFormat => 104
define error_msg_lspecInvalidResultFormat  => "You have specified an invalid result format"


define lspec_impl => type {
    // Tallying up the test suite
    data private suite    = array,
         private result   = string,
         private failures = '',
         private pendings = ''
    // Tally Getters
    public suite    => .'suite',
           result   => .'result',
           failures => .'failures',
           pendings => .'pendings'
         
    // Flow status
    data private inTestGroup  = false,
         private inTest       = false,
         private currentDepth = 0,
         private done         = false
    // Flow "Setters"
    public enterTestGroup => { .inTestGroup = true  },
           leaveTestGroup => { .inTestGroup = false },
           enterTest      => { .inTest = true  },
           leaveTest      => { .inTest = false },
           incGroupDepth  => .currentDepth++,
           decGroupDepth  => .currentDepth--,
           isStopped      => .done
    public stop => {
        fail_if(.inTestGroup,
            error_code_lspecStopInTestGroup, error_msg_lspecStopInTestGroup)
        .done = true
        .outputPendings
        .outputFailures
        .outputSummary
    }
         
    // Stats
    data private numTests     = 0,
         private numTestCases = 0,
         private numSuccesses = 0,
         private numFailures  = 0,
         private numPending   = 0,
         private runTime      = 0
    // Stats Getters and Setters
     public numTests     => .'numTests',
            numTestCases => .'numTestCases',
            numSuccesses => .'numSuccesses',
            numFailures  => .'numFailures',
            numPending   => .'numPending',
            runTime      => .'runTime',
            runTime(time::integer) => { .'runTime' += #time }
         
    // Configuration Options
    data private validFormats = (:'p', 'd'),
         private options      = map(
                    'resultsFormat'  = "p",
                    'indentor'       = "    ",
                    'suppressOutput' = false,
         )
    // Configuration Options Setters & Getters
    public options => .'options'
    public setOptions(new::map) => {
        with key in .'options'->keys 
        do {
            #new->contains(#key)? .'options'->find(#key) = #new->find(#key)
        }
    }
    public indentor => .options->find('indentor'),
           indentor(new::string) => { .options->find('indentor') = #new }
       
    public isOutputSuppressed =>   .options->find('suppressOutput'),
           suppressOutput     => { .options->find('suppressOutput') = true  },
           showOutput         => { .options->find('suppressOutput') = false }
           
    public resultsFormat => .options->find('resultsFormat')
    public formatResults(new::string) => {
        fail_if(not .validFormats->contains(#new), 
            error_code_lspecInvalidResultFormat, error_msg_lspecInvalidResultFormat)
        .options->find('resultsFormat') = #new
    }
    
    
    // Code to add test stuff to lspec test suite
    public addTestGroup(tg::lspec_testGroup) => {
        fail_if(.inTest, error_code_lspecTestGroupInTest, error_msg_lspecTestGroupInTest)

        lspec->isStopped? lspec_clearSuite
        
        if(.inTestGroup) => {
            .getCurrentTestGroup->groups->insert(#tg)
        else
            .suite->insert(#tg)
            #tg->run
        }
    }
    public addTest(test::lspec_test) => {
        fail_if(not .inTestGroup, error_code_lspecNotInTestGroup, error_msg_lspecNotInTestGroup)
        .getCurrentTestGroup->tests->insert(#test)
        .numTests++
    }
    public addTestCase => {
        fail_if(not .inTest, error_code_lspecNotInTest, error_msg_lspecNotInTest)
        .numTestCases++
    }
    
    
    // Test suite location methods
    private getCurrentTestGroup => {
        local(ret_val) = .suite->last
        loop(.currentDepth) => {
            #ret_val = #ret_val->groups->last
        }
        return #ret_val
    }
    public getCurrentTest => {
        return .getCurrentTestGroup->tests->get(.getCurrentTestGroup->currentTest)
    }
    
    
    // Completed test status code
    public testPending(test::lspec_test) => {
        .numPending++
        
        local(spacer)    = ' ' * (.numPending->asString->size + 2)
        
        .pendings == ''? .pendings += '\n\nPending:'
        
        .pendings += '\n\n' + .indentor + .numPending + ') ' + .getCurrentTestGroup->label + ' ' + #test->label
        .pendings += '\n' + .indentor + #spacer + .indentor + '# ' + #test->location->first + ' ' + #test->location->second
    }
    
    public testSucceeded => .numSuccesses++
    
    public testFailed(test::lspec_test, error=null) => {
        .numFailures++
        
        local(spacer)    = ' ' * (.numFailures->asString->size + 2)
        local(location)  = (not #error ? #test->failedTestCase->location | #test->location)
        local(line_num)  = integer(#location->first->split(':')->first)
        local(col_num)   = integer(#location->first->split(':')->second)
        local(code)
        
        if(#error) => {
            #code = 'Unexpcted Error!'
        else
            local(f) = file(#location->second, file_openRead, file_modeLine)
            #f->doWithClose => {
                loop(#line_num - 1) => {#f->get}
                #code = #f->get->sub(#col_num)
            }
        }
        
        .failures == ''? .failures += '\n\nFailures:'
        
        .failures += '\n\n' + .indentor + .numFailures + ') ' + .getCurrentTestGroup->label + ' ' + #test->label
        .failures += '\n' + .indentor + #spacer + 'Failure/Error: ' + #code
        if(#error) => {
            .failures += '\n' + .indentor + #spacer + .indentor + 'Error Code: ' + #error->first
            .failures += '\n' + .indentor + #spacer + .indentor + ' Error Msg: ' + #error->second
        else
            .failures += '\n' + .indentor + #spacer + .indentor + 'expected: ' + #test->failedTestCase->val_expected
            .failures += '\n' + .indentor + #spacer + .indentor + '     got: ' + #test->failedTestCase->val_actual
        }
        .failures += '\n' + .indentor + #spacer + '# ' + #location->first + ' ' + #location->second
    }
    
    
    // Test suite output code
    public output(value::string) => {
        .result += #value
        not .isOutputSuppressed? stdout(#value)
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
    
    public outputSummary => {
        local(summary) = '\n\nFinished in ' + .runTime + ' seconds\n'
        #summary += .numTests + ' test' + (.numTests == 1 ? '' | 's')
        #summary += ', ' + .numFailures + ' failure' + (.numFailures == 1 ? '' | 's')
        .numPending > 0?
            #summary += ', ' + .numPending + ' pending'
        .output(#summary)
    }
    public outputPendings => .output(.pendings)
    public outputFailures => .output(.failures)
}

define lspec => var(_lspec) || var(_lspec) := lspec_impl
// Could probably work around by throwing all the defaults into
// an onCreate method and then calling that, but would prefer not to
// as I'd have to remember to add every new data member to the oncreate
// even if it would be to just set it up with void
define lspec_clearSuite => {
    local(options) = lspec->options
    var(_lspec) = lspec_impl
    $_lspec->setOptions(#options)
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
        local(start) = micros
        lspec->enterTestGroup
        handle => { 
            lspec->leaveTestGroup
            lspec->runTime(micros - #start)
        }
        
        lspec->output(self)
        .block->invoke
        .currentTest = 0
        .tests->forEach => {
            .currentTest++
            #1->run
        }
        .groups->forEach => {
            lspec->incGroupDepth
            #1->run 
            lspec->decGroupDepth
        }
    }
}

define lspec_test => type {
    data public  label::string,
         public  block::capture,
         public  status::string,
         private location::pair,
         public  failedTestCase::expect
    
    // Getters
    public location => .'location'
    
    public onCreate(label, gb::capture) => {
        .label = #label->asString
        .block = #gb
        
        // We need to know where the test came from in case of errors
        // (Trying to get the error_stack in the handle method doesn't give us what we need)
        iterate(currentCapture->callStack->split('\n')) => {
            local(line_col)  = loop_value->sub(1,loop_value->find(' ') - 1)
            local(file_path) = loop_value->sub(1+loop_value->find(' '))
            if(currentCapture->callsite_file != #file_path) => {
                .location = pair(#line_col, #file_path)
                loop_abort
            }
        }
    }
    
    public run => {
        local(cur_failures) = lspec->numFailures
        local(cur_cases)    = lspec->numTestCases

        lspec->output(self)

        lspec->enterTest
        handle => { lspec->leaveTest }
        local(gb_error) = pair(0='No error')
        protect => {
            handle_error => { #gb_error = pair(error_code=error_msg) }
            .block->invoke
        }

        if(#gb_error->first != 0) => {
            failure_clear
            error_reset
            .status = 'fail'
            lspec->testFailed(self, #gb_error)
        else(lspec->numTestCases == #cur_cases)
            .status = 'pending'
            lspec->testPending(self)
        else(not .status)
            .status = 'success'
            lspec->testSucceeded
        else
            lspec->testFailed(self)
        }
        
        lspec->outputStatus(.status)
    }
}


define describe(label) => {
    local(me) = lspec_testGroup(#label, givenBlock)
    lspec->addTestGroup(#me)
}


define it(label) => {
    lspec->addTest(lspec_test(#label, givenBlock))
}


define expect => type {
    data private location,
         private val_expected,
         private val_actual
    // Want getters but not setters
    public location     => .'location'
    public val_expected => .'val_expected'
    public val_actual   => .'val_actual'
    
    public onCreate => {
        lspec->addTestCase
        
        // We need to know where the expect came from
        iterate(currentCapture->callStack->split('\n')) => {
            local(line_col)  = loop_value->sub(1,loop_value->find(' ') - 1)
            local(file_path) = loop_value->sub(1+loop_value->find(' '))
            if(currentCapture->callsite_file != #file_path) => {
                .location = pair(#line_col, #file_path)
                loop_abort
            }
        }
    }
    public onCreate(test_case::boolean) => {
        .onCreate
        not #test_case? .fail('true', 'false')
    }
    
    public fail(expected::string='', got::string='') => {
        .val_expected = #expected
        .val_actual   = #got
        lspec->getCurrentTest->status = 'fail'
        lspec->getCurrentTest->failedTestCase = self
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
    
    failure_clear
    error_reset
    
    if(#my_error->first != #code or #my_error->second != #msg) => {
        .fail(#code + ': ' + #msg,
              #my_error->first + ': ' + #my_error->second)
    }
}

define expect->stdout(expected::string) => {
    fail_if(not givenBlock,
        error_code_missingGivenBlock, error_msg_missingGivenBlock)
    
    // save stdout using ->dup
    // create a local conduit
    // close real stdout, reopen it on our write pipe using dup2 system call
    // make a file object here which we read stdout from
    local(save_stdout) = file_stdout->fd->dup
    local(readFd, writeFd) = io_file_pipe
    #writeFd->dup2(file_stdout->fd)
    local(read_test) = file(#readFd)
    
    handle => {
        // set stdout back using our save
        #save_stdout->dup2(file_stdout->fd)
    }
    
    givenBlock()
    
    // read test data
    local(did_read) = #read_test->readString
    
    #did_read != #expected? .fail(#expected, #did_read)
}