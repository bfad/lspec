/*
   Copyright 2011 Bradley Lindsay

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
/*
* Terminology:
*   expectation => each [expect] style statement
*   test        => each [it] statement
*   test group  => each [describe] statement
*   test suite  => Compilation of all the test groups [lspec->suite]
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


define lspec_testSuite => type {
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
            runTimeSec   => .'runTime' / 1000000.0,
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
    private currentTGroupLabels => {
        local(test_group) = .suite->last
        local(ret_val)    = ''
        loop(.currentDepth) => {
            #ret_val   += #test_group->label + ' '
            #test_group = #test_group->groups->last
        }
        #ret_val += #test_group->label
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
        
        .failures += '\n\n' + .indentor + .numFailures + ') ' + .currentTGroupLabels + ' ' + #test->label
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
        'p' == .resultsFormat? return
        
        local(out) = (.indentor * .currentDepth) + #tg->label
        .result != '' ? #out = '\n' + #out
        .output(#out)
    }
    public output(test::lspec_test) => {
        'p' == .resultsFormat? return
        
        local(out) = '\n' + (.indentor * .currentDepth) + .indentor + #test->label
        .output(#out)
    }
    
    public outputStatus(status::string) => {
        match(.resultsFormat) => {
        case('d')
            #status != 'fail'? return
            .output(' (FAILED - ' + .numFailures + ')')
        case('p')
            match(#status) => {
            case('pending') .output('*')
            case('success') .output('.')
            case('fail')    .output('F')
            }
        }
    }
    
    public outputSummary => {
        local(summary) = '\n\nFinished in ' + .runTimeSec + ' seconds\n'
        #summary += .numTests + ' test' + (.numTests == 1 ? '' | 's')
        #summary += ', ' + .numFailures + ' failure' + (.numFailures == 1 ? '' | 's')
        .numPending > 0?
            #summary += ', ' + .numPending + ' pending'
        .output(#summary)
    }
    public outputPendings => .output(.pendings)
    public outputFailures => .output(.failures)
}

/*
* Wrapper for the test suite.
* This allows us to create one universal test suite object per thread. All
* subsequent methods use this object, and so should users when necessary
*/
define lspec => var(_lspec) || $_lspec := lspec_testSuite

/*
* Method used to clear out the [lspec] test suite but leave formatting options in place.
*/
define lspec_clearSuite => {
    local(options) = lspec->options
    var(_lspec)    = lspec_testSuite
    $_lspec->setOptions(#options)
}


/*
* The test group type
*/
define lspec_testGroup => type {
    data private label::string,
         private block::capture,
         private tests  = array,
         private groups = array,
         private currentTest = 0
     // Getters
     public label       => .'label',
            tests       => .'tests',
            groups      => .'groups',
            currentTest => .'currentTest'
    
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

/*
* The test type
*/
define lspec_test => type {
    data private label::string,
         private block::capture,
         private status::string,
         private location::pair,
         private failedTestCase::expect
    // Getters
    public label          => .'label',
           status         => .'status',
           location       => .'location',
           failedTestCase => .'failedTestCase'
    // Setters
    public failed(test_case::expect)   => {
        .status = 'fail'
        .failedTestCase = #test_case
    }
    
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


/*
* Wrapper for getting a test group into the test suite
*/
define describe(label) => {
    lspec->addTestGroup(lspec_testGroup(#label, givenBlock))
}
define context(label) =>  {
    lspec->addTestGroup(lspec_testGroup(#label, givenBlock))
}


/*
* Wrapper for getting a test into the test suite
*/
define it(label) => {
    lspec->addTest(lspec_test(#label, givenBlock))
}


/*
* The base expect type that can be extended for test cases
* The base onCreate sets up everything that is needed before the test case runs
* The onCreate that takes a boolean is the simplest test case - asserting true
*/
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
    
    private fail(expected::string='', got::string='') => {
        .val_expected = #expected
        .val_actual   = #got
        lspec->getCurrentTest->failed(self)
    }
}

/*
* This expect statement catches any errors sent to its block and then
* compares the error with the given expected error.
*/
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

/*
* This expect statement catches whatever is sent to stdout in its block and
* compares that with the given expected result.
*/
define expect->stdout(expected::string) => {
    fail_if(not givenBlock,
        error_code_missingGivenBlock, error_msg_missingGivenBlock)
    
    // save stdout using ->dup
    // create a local conduit
    // close real stdout, reopen it on our write pipe using dup2 system call
    // make a file object here which we read stdout from
    local(save_stdout)     = file_stdout->fd->dup
    local(readFd, writeFd) = io_file_pipe
    #writeFd->dup2(file_stdout->fd)
    local(read_test) = file(#readFd)
    // set stdout back using our save
    handle => { #save_stdout->dup2(file_stdout->fd) }
    
    givenBlock()
    
    local(did_read) = #read_test->readString
    
    #did_read != #expected? .fail(#expected, #did_read)
}