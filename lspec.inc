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
define error_msg_lspecStopInTestGroup  => "Cannot stop while in a test group"
    
define error_code_lspecInvalidResultFormat => 104
define error_msg_lspecInvalidResultFormat  => "You have specified an invalid result format"

define error_code_lspecTestGroupHasBeforeEach => 105
define error_msg_lspecTestGroupHasBeforeEach  => "You have already specified a [beforeEach] block"

define error_code_lspecTestGroupHasAfterEach => 106
define error_msg_lspecTestGroupHasAfterEach  => "You have already specified an [afterEach] block"

define error_code_lspecTestGroupHasBeforeAll => 107
define error_msg_lspecTestGroupHasBeforeAll  => "You have already specified a [beforeAll] block"

define error_code_lspecTestGroupHasAfterAll => 108
define error_msg_lspecTestGroupHasAfterAll  => "You have already specified an [afterAll] block"

define error_code_lspecSharedTestGroupAlreadyExists => 109
define error_msg_lspecSharedTestGroupAlreadyExists  => "The specified shared test group already exists"

define error_code_lspecSharedTestGroupNotFound => 110
define error_msg_lspecSharedTestGroupNotFound  => "The specified shared test group was not found"


define lspec_testSuite => type {
    // Tallying up the test suite
    data private suite    = array,
         private shared   = map,
         private result   = string,
         private failures = '',
         private pendings = ''
    // Tally Getters
    public suite    => .'suite',
           shared   => .'shared',
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
                            // p => Progress, default
                            // d => Documentation
         private options      = map(
                    'resultsFormat'  = "p",
                    'indentor'       = "    ",
                    'suppressOutput' = false,
                    'colorOutput'    = false,
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

    public isOutputInColor =>   .options->find(`colorOutput`),
           noColor         => { .options->find(`colorOutput`) = false },
           useColor        => { .options->find(`colorOutput`) = true  }
           
    public resultsFormat => .options->find('resultsFormat')
    public formatResults(new::string) => {
        fail_if(not .validFormats->contains(#new),
            error_code_lspecInvalidResultFormat, error_msg_lspecInvalidResultFormat)
        .options->find('resultsFormat') = #new
    }
    
    // Color Stuff
    data terminalColors = map(
            `normal`     = decode_base64('Gw==') + '[0m',
            `boldRed`    = decode_base64('Gw==') + '[1;31;49m',
            `boldGreen`  = decode_base64('Gw==') + '[1;32;49m',
            `boldYellow` = decode_base64('Gw==') + '[1;33;49m',
            `boldCyan`   = decode_base64('Gw==') + '[1;36;49m',
    )
    public outputColor(color::string) => {
        not .isOutputInColor? return ''

        return string(.'terminalColors'->get(#color))
    }


    // Code to add test stuff to lspec test suite
    public addSharedGroup(label::string, block::capture) => {
        fail_if(.inTest, error_code_lspecTestGroupInTest, error_msg_lspecTestGroupInTest)
        .shared->insert(pair(#label, #block))
    }
    public addTestGroup(tg::lspec_testGroup) => {
        fail_if(.inTest, error_code_lspecTestGroupInTest, error_msg_lspecTestGroupInTest)
        
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
    public addBeforeAll(block::capture) => {
        fail_if(not .inTestGroup, error_code_lspecNotInTestGroup, error_msg_lspecNotInTestGroup)
        .getCurrentTestGroup->beforeAll(#block)
    }
    public addBeforeEach(block::capture) => {
        fail_if(not .inTestGroup, error_code_lspecNotInTestGroup, error_msg_lspecNotInTestGroup)
        .getCurrentTestGroup->beforeEach(#block)
    }
    public addAfterAll(block::capture) => {
        fail_if(not .inTestGroup, error_code_lspecNotInTestGroup, error_msg_lspecNotInTestGroup)
        .getCurrentTestGroup->afterAll(#block)
    }
    public addAfterEach(block::capture) => {
        fail_if(not .inTestGroup, error_code_lspecNotInTestGroup, error_msg_lspecNotInTestGroup)
        .getCurrentTestGroup->afterEach(#block)
    }
    
    
    // Test suite location methods
    public getCurrentTestGroup => {
        local(ret_val) = .suite->last
        loop(.currentDepth) => {
            #ret_val = #ret_val->groups->get(#ret_val->currentGroup)
        }
        return #ret_val
    }
    private currentTGroupLabels => {
        local(test_group) = .suite->last
        local(ret_val)    = ''
        loop(.currentDepth) => {
            #ret_val   += #test_group->label + ' '
            #test_group = #test_group->groups->get(#test_group->currentGroup)
        }
        #ret_val += #test_group->label
        return #ret_val
    }
    public getCurrentTest => {
        return .getCurrentTestGroup->tests->get(.getCurrentTestGroup->currentTest)
    }
    public runCurrentBeforeEach => {
        local(test_group) = .suite->last
        loop(.currentDepth) => {
            #test_group->beforeEach->invoke
            #test_group = #test_group->groups->get(#test_group->currentGroup)
        }
        #test_group->beforeEach->invoke
    }
    
    
    // Completed test status code
    public testPending(test::lspec_test) => {
        .numPending++
        
        local(spacer)    = ' ' * (.numPending->asString->size + 2)
        
        .pendings == ''? .pendings += '\n\n' + .outputColor(`boldYellow`) + 'Pending:' + .outputColor(`normal`)
        
        .pendings += '\n\n' + .indentor + .numPending + ') ' + .getCurrentTestGroup->label + ' ' + #test->label
        .pendings += '\n' + .indentor + #spacer + .indentor + '# ' + #test->location->first + ' ' + #test->location->second
    }
    
    public testSucceeded => .numSuccesses++
    
    public testFailed(test::lspec_test, error=null) => {
        .numFailures++
        
        local(spacer)    = ' ' * (.numFailures->asString->size + 2)
        local(location)  = (not #error? #test->failedTestCase->location | #test->location)
        local(line_num)  = integer(#location->first->split(':')->first)
        local(col_num)   = integer(#location->first->split(':')->second)
        local(code)
        
        if(#error) => {
            #code = 'Unexpected Error!'
        else
            local(f) = file(#location->second, file_openRead, file_modeLine)
            #f->doWithClose => {
                loop(#line_num - 1) => {#f->get}
                #code = #f->get->sub(#col_num)
            }
        }
        
        .failures == ''? .failures += '\n\n' + .outputColor(`boldRed`) + 'Failures:' + .outputColor(`normal`)
        
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
        .result != ''
            ? #out = '\n' + #out
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
            case('pending') .output(.outputColor(`boldYellow`) + '*' + .outputColor(`normal`))
            case('success') .output(.outputColor(`boldGreen`)  + '.' + .outputColor(`normal`))
            case('fail')    .output(.outputColor(`boldRed`)    + 'F' + .outputColor(`normal`))
            }
        }
    }
    
    public outputSummary => {
        local(summary) = '\n\nFinished in ' + .runTimeSec + ' seconds\n'
        #summary->append(''   + .outputColor(`boldCyan`) + .numTests    + .outputColor(`normal`) + ' test'    + (.numTests    == 1 ? '' | 's'))
        #summary->append(', ' + .outputColor(`boldRed`)  + .numFailures + .outputColor(`normal`) + ' failure' + (.numFailures == 1 ? '' | 's'))
        .numPending > 0?
            #summary->append(', ' + .outputColor(`boldYellow`) + .numPending + .outputColor(`normal`) + ' pending')
        .output(#summary + '\n')
    }
    public outputPendings => .output(.pendings)
    public outputFailures => .output(.failures)
}

/*
* Wrapper for the test suite.
* This allows us to create one universal test suite object per thread. All
* subsequent methods use this object, and so should users when necessary.
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
         private suite::lspec_testSuite,
         private tests  = array,
         private groups = array,
         private beforeAll::capture,
         private beforeEach::capture,
         private afterAll::capture,
         private afterEach::capture,
         private parentGroup  = null,
         private currentTest  = 0,
         private currentGroup = 0
    // Getters
     public label        => .'label',
            parentSuite  => .'suite',
            tests        => .'tests',
            groups       => .'groups',
            beforeEach   => .'beforeEach',
            afterEach    => .'afterEach',
            currentTest  => .'currentTest',
            currentGroup => .'currentGroup'
    
    public onCreate(label, gb::capture, ts::lspec_testSuite=lspec) => {
        .label = #label->asString
        .block = #gb->asCopyDeep
        .suite = #ts
    }
    
    public numTests => .tests->size
    
    public beforeAll(block::capture) => {
        fail_if(.beforeAll,
            error_code_lspecTestGroupHasBeforeAll, error_msg_lspecTestGroupHasBeforeAll
        )
        .beforeAll = #block
        #block->invoke
    }
    
    public beforeEach(block::capture) => {
        fail_if(.beforeEach,
            error_code_lspecTestGroupHasBeforeEach, error_msg_lspecTestGroupHasBeforeEach
        )
        .beforeEach = #block
    }
    
    public afterAll(block::capture) => {
        fail_if(.afterAll,
            error_code_lspecTestGroupHasAfterAll, error_msg_lspecTestGroupHasAfterAll
        )
        .afterAll = #block
    }
    
    public afterEach(block::capture) => {
        fail_if(.afterEach,
            error_code_lspecTestGroupHasAfterEach, error_msg_lspecTestGroupHasAfterEach
        )
        .afterEach = #block
    }
    
    public runAfterEaches => {
        .afterEach->invoke
        .parentGroup? .parentGroup->runAfterEaches
    }
    
    public run => {
        local(start) = micros
        .parentSuite->enterTestGroup
        handle => {
            .afterAll->invoke
            .parentSuite->leaveTestGroup
            not .parentGroup? .parentSuite->runTime(micros - #start)
        }
        
        .parentSuite->output(self)
        .block->invoke
        .currentTest = 0
        .tests->forEach => {
            .currentTest++
            .parentSuite->runCurrentBeforeEach
            #1->run
            .runAfterEaches
        }
        .currentGroup = 0
        .groups->forEach => {
            .currentGroup++
            .parentSuite->incGroupDepth
            #1->parentGroup = self
            #1->run
            .parentSuite->decGroupDepth
        }
    }
}

/*
* The test type
*/
define lspec_test => type {
    data private label::string,
         private block::capture,
         private suite::lspec_testSuite,
         private status::string,
         private location::pair,
         private failedTestCase::expect
    // Getters
    public label          => .'label',
           parentSuite    => .'suite',
           status         => .'status',
           location       => .'location',
           failedTestCase => .'failedTestCase'
    // Setters
    public failed(test_case::expect) => {
        .status = 'fail'
        .failedTestCase = #test_case
    }
    
    public onCreate(label, gb::capture, ts::lspec_testSuite=lspec) => {
        .label = #label->asString
        .block = #gb
        .suite = #ts
        
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
        local(cur_failures) = .parentSuite->numFailures
        local(cur_cases)    = .parentSuite->numTestCases

        .parentSuite->output(self)

        .parentSuite->enterTest
        handle => { .parentSuite->leaveTest }
        local(gb_error) = pair(0='No error')
        protect => {
            handle_error => { #gb_error = pair(error_code=error_msg) }
            .block->invoke
        }

        if(#gb_error->first != 0) => {
            failure_clear
            error_reset
            .status = 'fail'
            .parentSuite->testFailed(self, #gb_error)
        else(.parentSuite->numTestCases == #cur_cases)
            .status = 'pending'
            .parentSuite->testPending(self)
        else(not .status)
            .status = 'success'
            .parentSuite->testSucceeded
        else
            .parentSuite->testFailed(self)
        }
        
        .parentSuite->outputStatus(.status)
    }
}


/*
* Wrapper for getting a test group into the test suite
*/
define describe(label) => {
    lspec->isStopped? lspec_clearSuite
    lspec->addTestGroup(lspec_testGroup(#label, givenBlock))
}
define context(label) =>  {
    lspec->isStopped? lspec_clearSuite
    lspec->addTestGroup(lspec_testGroup(#label, givenBlock))
}

/*
* Wrappers for sharing test groups among other test groups
*/
define shared_examples(label) => {
    fail_if(lspec->shared->contains(#label),
        error_code_lspecSharedTestGroupAlreadyExists, error_msg_lspecSharedTestGroupAlreadyExists)
    lspec->isStopped? lspec_clearSuite
    lspec->addSharedGroup(#label, givenBlock)
}
define it_behaves_like(label) => {
    fail_if(not lspec->shared->contains(#label),
        error_code_lspecSharedTestGroupNotFound, error_msg_lspecSharedTestGroupNotFound)
    lspec->isStopped? lspec_clearSuite
    lspec->addTestGroup(lspec_testGroup('behaves like ' + #label, lspec->shared->get(#label)))
}


/*
* Wrapper for getting a test into the test suite
*/
define it(label) => {
    lspec->addTest(lspec_test(#label, givenBlock))
}


/*
* Before and after hooks
*/
define beforeAll => {
    lspec->addBeforeAll(givenBlock)
}
define beforeEach => {
    lspec->addBeforeEach(givenBlock)
}
define afterAll => {
    lspec->addAfterAll(givenBlock)
}
define afterEach => {
    lspec->addAfterEach(givenBlock)
}


/*
* The base expect type that can be extended for test cases
* The base onCreate sets up everything that is needed before the test case runs
* The onCreate that takes a boolean is the simplest test case - asserting true
*/
define expect => type {
    data private suite::lspec_testSuite,
         private location,
         private val_expected,
         private val_actual
    // Want getters but not setters
    public parentSuite  => .'suite',
           location     => .'location',
           val_expected => .'val_expected',
           val_actual   => .'val_actual'
    
    public onCreate(ts::lspec_testSuite=lspec) => {
        .suite = #ts
        .parentSuite->addTestCase
        
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
    public onCreate(test_case::boolean, expected::string='true', got::string='false') => {
        .onCreate
        not #test_case? .fail(#expected, #got)
    }
    public onCreate(expected, got) => {
        .onCreate(#expected == #got, #expected->asString, #got->asString)
    }
    
    private fail(expected::string='', got::string='') => {
        .val_expected = #expected
        .val_actual   = #got
        .parentSuite->getCurrentTest->failed(self)
    }
}

/*
* This expect statement expects a null value to be passed into it.
*/
define expect->null(value) => {
    not #value->isA(::null)?
        .fail('null', '"' + #value + '"')
}

/*
* This expect statement runs the ->isA test on the value passed into it.
* This should be used to check if the value is an object with the specified parent type or trait.
* To check if the value is a specific type, I recommend using the following code:
*    expect(::type, #value->type)
*/
define expect->valueIsA(check::tag, value) => {
    not #value->isA(#check)?
        .fail(#check->asString, #value->type->asString)
}
define expect->valueIsA(value, check::tag) => expect->valueIsA(#check, #value) // original argument order

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
* This expect statement catches any errors sent to its block and then
* compares the error code with the given expected error code.
*/
define expect->errorCode(code::integer) => {
    fail_if(not givenBlock,
        error_code_missingGivenBlock, error_msg_missingGivenBlock)
    
    local(my_error) = error_code_noerror
    protect => {
        handle_error => { #my_error = error_code }
        givenBlock()
    }
    
    failure_clear
    error_reset
    
    if(#my_error != #code) => {
        .fail(string(#code), string(#my_error))
    }
}

/*
* This expect statement catches any errors sent to its block and then
* compares the error message with the given expected error message.
*/
define expect->errorMsg(msg::string) => {
    fail_if(not givenBlock,
        error_code_missingGivenBlock, error_msg_missingGivenBlock)
    
    local(my_error) = error_msg_noerror
    protect => {
        handle_error => { #my_error = error_msg }
        givenBlock()
    }
    
    failure_clear
    error_reset
    
    if(#my_error != #msg) => {
        .fail(#msg, #my_error)
    }
}

/*
* This expect statement passes if there is any error in the
* givenBlock and fails there is no error.
*/
define expect->error => {
    fail_if(not givenBlock,
        error_code_missingGivenBlock, error_msg_missingGivenBlock)

    local(my_error) = pair(error_code_noerror=error_msg_noerror)
    protect => {
        handle_error => { #my_error = pair(error_code=error_msg) }
        givenBlock()
    }

    failure_clear
    error_reset
    
    if(#my_error->first == error_code_noerror and #my_error->second == error_msg_noerror) => {
        .fail('Any Error', #my_error->first + ': ' + #my_error->second)
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
    
    #did_read != #expected
        ? .fail(#expected, #did_read)
}
