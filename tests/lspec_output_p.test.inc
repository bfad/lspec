/*
* Testing the resulting output of test suite
*
* Must be run from the lspec.test.inc file
*/
local(test_desc) = "Properly outputs simply a test group in progress format"
lspec_setupForTesting
lspec->formatResults('p')
describe("Something") => {}
if(lspec->result != "") => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    stdoutnl(lspec->result)
    abort
}



#test_desc = "Properly outputs sub-test groups in progress format"
lspec_setupForTesting
lspec->formatResults('p')
describe("Something") => {
    it("Does something") => {}
    describe("As an aspect") => {
        describe("Sub-aspect") => {}
        it("Does something cool") => {}
    }
    it("Does something else") => {}
}
local(expected) = '***'
if(lspec->result != #expected) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "Properly outputs a pending test in progress format"
lspec_setupForTesting
lspec->formatResults('p')
describe("Something") => {
    it("does something") => {}
}
lspec->stop
local(expected) = '*

Pending:

' + lspec->indentor + '1) Something does something
' + lspec->indentor + '   ' + lspec->indentor + '# 43:5 ' + currentCapture->callsite_file + '

Finished in ' + lspec->runTime + ' seconds
1 test, 0 failures, 1 pending'
if(lspec->result != #expected) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "Properly outputs a basic successful test in progress format"
lspec_setupForTesting
lspec->formatResults('p')
describe("Something") => {
    it("does something") => {
        expect(true)
    }
}
lspec->stop
local(expected) = '.

Finished in ' + lspec->runTime + ' seconds
1 test, 0 failures'
if(lspec->result != #expected) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "Properly outputs a basic failed test in progress format"
lspec_setupForTesting
lspec->formatResults('p')
describe("Something") => {
    it("does something") => {
        expect(false)
    }
}
lspec->stop
local(expected) = 'F

Failures:

' + lspec->indentor + '1) Something does something
' + lspec->indentor + '   Failure/Error: expect(false)
' + lspec->indentor + '   ' + lspec->indentor + 'expected: true
' + lspec->indentor + '   ' + lspec->indentor + '     got: false
' + lspec->indentor + '   # 89:9 ' + currentCapture->callsite_file + '

Finished in ' + lspec->runTime + ' seconds
1 test, 1 failure'
if(lspec->result != #expected) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}