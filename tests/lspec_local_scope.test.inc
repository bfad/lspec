// These test define what happens with locals
local(test_desc) = "The value of a local defined outside a testGroup is accessible inside it."
lspec_setupForTesting
var(success) = false
local(test)  = true
describe("Something") => {
    $success = #test
}
#test = false
if(not $success) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "For a local defined outside a testGroup, changing the value inside doesn't change it outside."
lspec_setupForTesting
local(success) = true
describe("Something") => {
    #success = false
}
if(not #success) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "The value of a local for a child testGroup is the value when the child is declared though they are run later."
lspec_setupForTesting
var(expected1) = null
var(expected2) = null
var(expected3) = null
describe("Something") => {
    local(start1) = 'foo'
    describe("as something") => {
        $expected1 = #start1
        $expected2 = 'foo'
    }
    #start1    = 'bar'
    $expected2 = 'bar'
    $expected3 = #start1
}
if('bar' != $expected3) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    stdoutnl('Expected: bar')
    stdoutnl('Received: ' + $expected3)
    abort
}
if('foo' != $expected2) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    stdoutnl('Expected: foo')
    stdoutnl('Received: ' + $expected2)
    abort
}
if('foo' != $expected1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    stdoutnl('Expected: foo')
    stdoutnl('Received: ' + $expected1)
    abort
}



#test_desc = "The value of a local for a test in a testGroup is the last value of the parent testGroup"
lspec_setupForTesting
var(expected) = null
describe("Something") => {
    local(startval) = 'a'
    describe("as something") => {
        #startval = 'b'
        describe("in some context") => {
            #startval = 'c'
            it("does something") => {
                $expected = #startval
            }
            #startval = 'd'
        }
        #startval = 'e'
    }
    #startval = 'f'
}
if('d' != $expected) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    stdoutnl('Expected: d')
    stdoutnl('Received: ' + $expected)
    abort
}



#test_desc = "The value of a local for a test in a testGroup can be affected by the testGroups beforeEach"
lspec_setupForTesting
var(expected) = null
describe("Something") => {
    local(startval) = 'a'
    describe("as something") => {
        #startval = 'b'
        describe("in some context") => {
            #startval = 'c'
            
            beforeEach => { #startval = 'd' }
            
            it("does something") => {
                $expected = #startval
            }
            #startval = 'e'
        }
        #startval = 'f'
    }
    #startval = 'g'
}
if('d' != $expected) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    stdoutnl('Expected: d')
    stdoutnl('Received: ' + $expected)
    abort
}



#test_desc = "The value of a local for a test in a testGroup can be affected by the testGroups beforeAll"
lspec_setupForTesting
var(expected) = null
describe("Something") => {
    local(startval) = 'a'
    describe("as something") => {
        #startval = 'b'
        describe("in some context") => {
            #startval = 'c'
            
            beforeAll => { #startval = 'd' }
            
            it("does something") => {
                $expected = #startval
            }
            //#startval = 'e'
        }
        #startval = 'f'
    }
    #startval = 'g'
}
if('d' != $expected) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    stdoutnl('Expected: d')
    stdoutnl('Received: ' + $expected)
    abort
}



#test_desc = "The value of a local for a test in a testGroup can be affected by the testGroups afterEach"
lspec_setupForTesting
var(expected1) = null
var(expected2) = null
describe("Something") => {
    local(startval) = 'a'
    describe("as something") => {
        #startval = 'b'
        describe("in some context") => {
            #startval = 'c'
            
            afterEach => { #startval = 'd' }
            
            it("does something") => {
                $expected1 = #startval
            }
            it("does something else") => {
                $expected2 = #startval
            }
            #startval = 'e'
        }
        #startval = 'f'
    }
    #startval = 'g'
}
if('e' != $expected1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    stdoutnl('Expected: e')
    stdoutnl('Received: ' + $expected1)
    abort
}
if('d' != $expected2) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    stdoutnl('Expected: d')
    stdoutnl('Received: ' + $expected2)
    abort
}



#test_desc = "The value of a local for a test in a testGroup isn't affected by the testGroups afterAll"
lspec_setupForTesting
var(expected) = null
describe("Something") => {
    local(startval) = 'a'
    describe("as something") => {
        #startval = 'b'
        describe("in some context") => {
            #startval = 'c'
            
            afterAll => { #startval = 'e' }
            
            it("does something") => {
                $expected = #startval
            }
            #startval = 'd'
        }
        #startval = 'f'
    }
    #startval = 'g'
}
if('d' != $expected) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    stdoutnl('Expected: d')
    stdoutnl('Received: ' + $expected)
    abort
}