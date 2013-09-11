/*
* Testing the resulting output of test suite in color
*
* Must be run from the lspec.test.inc file
*/
local(test_desc) = "Properly outputs simply a test group in progress format"
lspec_setupForTesting
lspec->formatResults('p')&useColor
describe("Something") => {}
if(lspec->result != "") => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    stdoutnl(lspec->result)
    abort
}



#test_desc = "Properly outputs sub-test groups in progress format"
lspec_setupForTesting
lspec->formatResults('p')&useColor
describe("Something") => {
    it("Does something") => {}
    describe("As an aspect") => {
        describe("Sub-aspect") => {}
        it("Does something cool") => {}
    }
    it("Does something else") => {}
}
local(expected) = ($termColor(`boldYellow`) + '*' + $termColor(`normal`)) * 3
if(lspec->result != #expected) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "Properly outputs a pending test in progress format"
lspec_setupForTesting
lspec->formatResults('p')&useColor
describe("Something") => {
    it("does something") => {}
}
lspec->stop
local(expected) = $termColor(`boldYellow`) + '*' + $termColor(`normal`) + '

' + $termColor(`boldYellow`) + 'Pending:' + $termColor(`normal`) + '

' + lspec->indentor + '1) Something does something
' + lspec->indentor + '   ' + lspec->indentor + '# 43:5 ' + currentCapture->callsite_file + '

Finished in ' + lspec->runTimeSec + ' seconds
' + $termColor(`boldCyan`) + '1' + $termColor(`normal`) + ' test, ' + $termColor(`boldRed`) + '0' + $termColor(`normal`) + ' failures, ' + $termColor(`boldYellow`) + '1' + $termColor(`normal`) + ' pending'
if(lspec->result != #expected) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "Properly outputs a basic successful test in progress format"
lspec_setupForTesting
lspec->formatResults('p')&useColor
describe("Something") => {
    it("does something") => {
        expect(true)
    }
}
lspec->stop
local(expected) = $termColor(`boldGreen`) + '.' + $termColor(`normal`) + '

Finished in ' + lspec->runTimeSec + ' seconds
' + $termColor(`boldCyan`) + '1' + $termColor(`normal`) + ' test, ' + $termColor(`boldRed`) + '0' + $termColor(`normal`) + ' failures'
if(lspec->result != #expected) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "Properly outputs a basic failed test in progress format"
lspec_setupForTesting
lspec->formatResults('p')&useColor
describe("Something") => {
    it("does something") => {
        expect(false)
    }
}
lspec->stop
local(expected) = $termColor(`boldRed`) + 'F' + $termColor(`normal`) + '

' + $termColor(`boldRed`) + 'Failures:' + $termColor(`normal`) + '

' + lspec->indentor + '1) Something does something
' + lspec->indentor + '   Failure/Error: expect(false)
' + lspec->indentor + '   ' + lspec->indentor + 'expected: true
' + lspec->indentor + '   ' + lspec->indentor + '     got: false
' + lspec->indentor + '   # 89:9 ' + currentCapture->callsite_file + '

Finished in ' + lspec->runTimeSec + ' seconds
' + $termColor(`boldCyan`) + '1' + $termColor(`normal`) + ' test, ' + $termColor(`boldRed`) + '1' + $termColor(`normal`) + ' failure'
if(lspec->result != #expected) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}