local(test_desc) = "Given a describe with 2 [it] blocks, it should return 2 as the number of tests"
lspec_setupForTesting
describe("Something") => {
    it("Does something") => {}
    it("Does something else") => {}
}
if(lspec->suite->first->numTests != 2) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "[describe] sub-test groups properly handled at the end of tests & outputted correctly"
lspec_setupForTesting
var(flag1 = false, flag2 = false)
describe("Something") => {
    it("Does something") => {}
    describe("As an aspect") => {
        $flag1 = true
        describe("Sub-aspect") => {
            $flag2 = true
        }
        it("Does something cool") => {
            $flag2 = false
        }
    }
    it("Does something else") => {
        $flag1 = false
        $flag2 = false
    }
}
if(not $flag1 or not $flag2) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "Given a [context] with 2 [it] blocks, it should return 2 as the number of tests"
lspec_setupForTesting
context("Something") => {
    it("Does something") => {}
    it("Does something else") => {}
}
if(lspec->suite->first->numTests != 2) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "[context] sub-test groups properly handled at the end of tests & outputted correctly"
lspec_setupForTesting
var(flag1 = false, flag2 = false)
context("Something") => {
    it("Does something") => {}
    context("As an aspect") => {
        $flag1 = true
        context("Sub-aspect") => {
            $flag2 = true
        }
        it("Does something cool") => {
            $flag2 = false
        }
    }
    it("Does something else") => {
        $flag1 = false
        $flag2 = false
    }
}
if(not $flag1 or not $flag2) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "It should throw an error if an [beforeEach] method isn't inside a [describe] method"
lspec_setupForTesting
try => {
    var(success) = false
    handle(error_code == error_code_lspecNotInTestGroup &&
           error_msg  == error_msg_lspecNotInTestGroup) => 
    {
        failure_clear
        error_reset
        $success = true
    }
    
    beforeEach => {}
    if(not $success) => {
        stdoutnl('Test Failed\n' + #test_desc)
        stdoutnl(currentCapture->callStack->split('\n')->first)
        abort
    }
}

$success = false
describe("Something") => {
    beforeEach => {}
    $success = true
}
if(not $success) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "It should throw an error if try to add a second [beforeEach] method"
lspec_setupForTesting
var(success) = false
describe("Something") => {
    beforeEach => {}
    try => {
        handle(error_code == error_code_lspecTestGroupHasBeforeEach &&
               error_msg  == error_msg_lspecTestGroupHasBeforeEach) => 
        {
            failure_clear
            error_reset
            $success = true
        }
        beforeEach => {}
    }
}
if(not $success) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "The [beforeEach] code should be called before a test group"
lspec_setupForTesting
var(num_calls) = 0
var(expected) = null
describe("Something") => {
    beforeEach => {
        $num_calls++
    }
    it("does one thing") => {
        $expected = $num_calls
    }
}
if(1 != $expected) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(1 != $num_calls) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "The [beforeEach] code should be called before each test in a test group"
lspec_setupForTesting
var(num_calls) = 0
describe("Something") => {
    beforeEach => {
        $num_calls++
    }
    it("does one thing") => {}
    it("does two thing") => {}
    it("does six thing") => {}
}
if(3 != $num_calls) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "Tests inside nested test groups call each [beforeEach] in outside-in order"
lspec_setupForTesting
var(result) = array
describe("Something") => {
    beforeEach => {
        $result->insert(1)
    }
    it("does something") => {}
    describe("as something") => {
        beforeEach => {
            $result->insert(2)
        }
        it("does something") => {}
        describe("in this context") => {
            beforeEach => {
                $result->insert(3)
            }
            it("does something") => {}
        }
    }
}
local(expected) = array(1,1,2,1,2,3)
if(#expected != $result) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "It should throw an error if an [afterEach] method isn't inside a [describe] method"
lspec_setupForTesting
try => {
    var(success) = false
    handle(error_code == error_code_lspecNotInTestGroup &&
           error_msg  == error_msg_lspecNotInTestGroup) => 
    {
        failure_clear
        error_reset
        $success = true
    }
    
    afterEach => {}
    if(not $success) => {
        stdoutnl('Test Failed\n' + #test_desc)
        stdoutnl(currentCapture->callStack->split('\n')->first)
        abort
    }
}

$success = false
describe("Something") => {
    afterEach => {}
    $success = true
}
if(not $success) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "It should throw an error if try to add a second [afterEach] method"
lspec_setupForTesting
var(success) = false
describe("Something") => {
    afterEach => {}
    try => {
        handle(error_code == error_code_lspecTestGroupHasAfterEach &&
               error_msg  == error_msg_lspecTestGroupHasAfterEach) => 
        {
            failure_clear
            error_reset
            $success = true
        }
        afterEach => {}
    }
}
if(not $success) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "The [afterEach] code should be called after a test group"
lspec_setupForTesting
var(num_calls) = 0
var(expected) = null
describe("Something") => {
    afterEach => {
        $num_calls++
    }
    it("does one thing") => {
        $expected = $num_calls
    }
}
if(0 != $expected) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(1 != $num_calls) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "The [afterEach] code should be called after each test in a test group"
lspec_setupForTesting
var(num_calls) = 0
describe("Something") => {
    afterEach => {
        $num_calls++
    }
    it("does one thing") => {}
    it("does two thing") => {}
    it("does six thing") => {}
}
if(3 != $num_calls) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "Tests inside nested test groups call each [afterEach] in inside-out order"
lspec_setupForTesting
var(result) = array
describe("Something") => {
    afterEach => {
        $result->insert(1)
    }
    it("does something") => {}
    describe("as something") => {
        afterEach => {
            $result->insert(2)
        }
        it("does something") => {}
        describe("in this context") => {
            afterEach => {
                $result->insert(3)
            }
            it("does something") => {}
        }
    }
}
local(expected) = array(1,2,1,3,2,1)
if(#expected != $result) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "It should throw an error if a [beforeAll] method isn't inside a [describe] method"
lspec_setupForTesting
try => {
    var(success) = false
    handle(error_code == error_code_lspecNotInTestGroup &&
           error_msg  == error_msg_lspecNotInTestGroup) => 
    {
        failure_clear
        error_reset
        $success = true
    }
    
    beforeAll => {}
    if(not $success) => {
        stdoutnl('Test Failed\n' + #test_desc)
        stdoutnl(currentCapture->callStack->split('\n')->first)
        abort
    }
}

$success = false
describe("Something") => {
    beforeAll => {}
    $success = true
}
if(not $success) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "It should throw an error if try to add a second [beforeAll] method"
lspec_setupForTesting
var(success) = false
describe("Something") => {
    beforeAll => {}
    try => {
        handle(error_code == error_code_lspecTestGroupHasBeforeAll &&
               error_msg  == error_msg_lspecTestGroupHasBeforeAll) => 
        {
            failure_clear
            error_reset
            $success = true
        }
        beforeAll => {}
    }
}
if(not $success) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "The [beforeAll] code should be called before a test group"
lspec_setupForTesting
var(num_calls) = 0
var(expected) = null
describe("Something") => {
    beforeAll => {
        $num_calls++
    }
    it("does one thing") => {
        $expected = $num_calls
    }
}
if(1 != $expected) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(1 != $num_calls) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "The [beforeAll] code should be called once before all the tests in a test group"
lspec_setupForTesting
var(num_calls) = 0
describe("Something") => {
    beforeAll => {
        $num_calls++
    }
    it("does one thing") => {}
    it("does two thing") => {}
    it("does six thing") => {}
}
if(1 != $num_calls) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "The [beforeAll] is only called once before any nested test groups or tests"
lspec_setupForTesting
var(result) = array
describe("Something") => {
    beforeAll => {
        $result->insert(1)
    }
    it("does something") => {}
    describe("as something") => {
        beforeAll => {
            $result->insert(2)
        }
        it("does something") => {}
        describe("in this context") => {
            beforeAll => {
                $result->insert(3)
            }
            it("does something") => {}
        }
    }
}
local(expected) = array(1,2,3)
if(#expected != $result) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "It should throw an error if an [afterAll] method isn't inside a [describe] method"
lspec_setupForTesting
try => {
    var(success) = false
    handle(error_code == error_code_lspecNotInTestGroup &&
           error_msg  == error_msg_lspecNotInTestGroup) => 
    {
        failure_clear
        error_reset
        $success = true
    }
    
    afterAll => {}
    if(not $success) => {
        stdoutnl('Test Failed\n' + #test_desc)
        stdoutnl(currentCapture->callStack->split('\n')->first)
        abort
    }
}

$success = false
describe("Something") => {
    afterAll => {}
    $success = true
}
if(not $success) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "It should throw an error if try to add a second [afterAll] method"
lspec_setupForTesting
var(success) = false
describe("Something") => {
    afterAll => {}
    try => {
        handle(error_code == error_code_lspecTestGroupHasAfterAll &&
               error_msg  == error_msg_lspecTestGroupHasAfterAll) => 
        {
            failure_clear
            error_reset
            $success = true
        }
        afterAll => {}
    }
}
if(not $success) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "The [afterAll] code should be called after a test group"
lspec_setupForTesting
var(num_calls) = 0
var(expected) = null
describe("Something") => {
    afterAll => {
        $num_calls++
    }
    it("does one thing") => {
        $expected = $num_calls
    }
}
if(0 != $expected) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(1 != $num_calls) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "The [afterAll] code should be called after all tests in a test group"
lspec_setupForTesting
var(num_calls) = 0
describe("Something") => {
    afterAll => {
        $num_calls++
    }
    it("does one thing") => {}
    it("does two thing") => {}
    it("does six thing") => {}
}
if(1 != $num_calls) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "Each [afterAll] in nested test groups in inside-out order"
lspec_setupForTesting
var(result) = array
describe("Something") => {
    afterAll => {
        $result->insert(1)
    }
    it("does something") => {}
    describe("as something") => {
        afterAll => {
            $result->insert(2)
        }
        it("does something") => {}
        describe("in this context") => {
            afterAll => {
                $result->insert(3)
            }
            it("does something") => {}
        }
    }
}
local(expected) = array(3,2,1)
if(#expected != $result) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "A [shared_examples] block is not run on its own"
lspec_setupForTesting
describe("Something") => {
	shared_examples('Shared examples') => {
		it("Does something") => {}
		it("Does something else") => {}
	}
}
if(lspec->suite->first->numTests != 0) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "[it_behaves_like] only runs tests that match its label"
lspec_setupForTesting
var(count = 0, flag = false)
describe("Something") => {
	shared_examples('Shared examples') => {
		$count++
	}
	shared_examples('Other examples') => {
		$flag = true
	}
	context('An aspect') => {
		it_behaves_like('Shared examples')
	}
	context('Another aspect') => {
		it_behaves_like('Shared examples')
	}
}
if($count != 2 or $flag) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "An error is thrown if a [shared_examples] is defined twice"
lspec_setupForTesting
var(success) = false
describe("Something") => {
	shared_examples('Shared examples') => {
		it("Does something") => {}
	}
    try => {
        handle(error_code == error_code_lspecSharedTestGroupAlreadyExists &&
               error_msg  == error_msg_lspecSharedTestGroupAlreadyExists) =>
        {
            failure_clear
            error_reset
            $success = true
        }
		shared_examples('Shared examples') => {
			it("Does something") => {}
		}
    }
}
if(not $success) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "An error is thrown if [it_behaves_like] requests a non-existent shared example"
lspec_setupForTesting
var(success) = false
describe("Something") => {
    try => {
        handle(error_code == error_code_lspecSharedTestGroupNotFound &&
               error_msg  == error_msg_lspecSharedTestGroupNotFound) =>
        {
            failure_clear
            error_reset
            $success = true
        }
		it_behaves_like('Shared examples')
    }
}
if(not $success) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
