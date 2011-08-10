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
    data public suite    = array,
         public result   = string,
         public failures = '',
         
         // Flow status
         public  inTestGroup  = false,
         public  inTest       = false,
         public  currentDepth = 0,
         private done         = false,
         
         // Stats
         public numTests     = 0,
         public numTestCases = 0,
         public numSuccesses = 0,
         public numFailures  = 0,
         public numPending   = 0,
         public runTime      = 0,
         
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
    
    public outputSummary => {
        .output('\n\nFinished in ' + .runTime + ' seconds\n' + .numTests + ' test' + (.numTests == 1 ? '' | 's') + ', ' + .numFailures + ' failure' + (.numFailures == 1 ? '' | 's'))
    }
    
    public outputFailures => {
        .output(.failures)
    }
    
    
    public testFailed(test::lspec_test, error=null, location=null) => {
        not #location? #location = #test->failedTestCase->location
            
        local(spacer)    = ' ' * (.numFailures->asString->size + 2)
        local(file_path) = #location->second
        local(line_num)  = integer(#location->first->split(':')->first)
        local(col_num)   = integer(#location->first->split(':')->second)
        local(code)
        
        if(#error) => {
            #code = 'Unexpcted Error!'
        else
            local(f) = file(#file_path, file_openRead, file_modeLine)
            #f->doWithClose => {
                loop(#line_num - 1) => {#f->get}
                #code = #f->get->sub(#col_num)
            }
        }
        
        .failures == ''? .failures += '\n\nFailures:'
        
        .failures += '\n\n' + .indentor + .numFailures + ') ' + .getCurrentTestGroup->label + ' ' + #test->label
        .failures += '\n' + .indentor + #spacer + 'Failure/Error: ' + #code
        if(#error) => {
            .failures += '\n' + .indentor + #spacer + lspec->indentor + 'Error Code: ' + #error->first
            .failures += '\n' + .indentor + #spacer + lspec->indentor + ' Error Msg: ' + #error->second
        else
            .failures += '\n' + .indentor + #spacer + lspec->indentor + 'expected: ' + #test->failedTestCase->val_expected
            .failures += '\n' + .indentor + #spacer + lspec->indentor + '     got: ' + #test->failedTestCase->val_actual
        }
        .failures += '\n' + .indentor + #spacer + '# ' + #location->first + ' ' + #file_path
    }
    
    
    // Flow control
    public stop => {
        fail_if(.inTestGroup,
            error_code_lspecStopInTestGroup, error_msg_lspecStopInTestGroup)
        
        .done = true
        .outputFailures
        .outputSummary
    }
    public isStopped => .done
    
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
        local(start) = micros
        lspec->inTestGroup = true
        handle => { 
            lspec->inTestGroup = false
            lspec->runTime += micros - #start
        }
        
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
    data public  label::string,
         public  block::capture,
         public  status::string,
         private location::pair,
         public  failedTestCase::expect
    
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
            .status = 'fail'
            lspec->numFailures++
            lspec->testFailed(self, #gb_error, .location)
        else(lspec->numTestCases == #cur_cases)
            lspec->numPending++
            .status = 'pending'
        else(not .status)
            lspec->numSuccesses++
            .status = 'success'
        else
            lspec->numFailures++
            lspec->testFailed(self)
        }
        
        lspec->outputStatus(.status)
    }
}


define describe(label) => {
    fail_if(lspec->inTest, 
        error_code_lspecTestGroupInTest, error_msg_lspecTestGroupInTest)
    
    lspec->isStopped? lspec_clearSuite
    
    local(me) = lspec_testGroup(#label, givenBlock)
    lspec->addTestGroup(#me)
}


define it(label) => {
    fail_if(not lspec->inTestGroup, error_code_lspecNotInTestGroup, error_msg_lspecNotInTestGroup)
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
        fail_if(not lspec->inTest, error_code_lspecNotInTest, error_msg_lspecNotInTest)
        lspec->numTestCases++
        
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
    
    if(#my_error->first != #code or #my_error->second != #msg) => {
        .fail
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
    
    #did_read != #expected? .fail
}