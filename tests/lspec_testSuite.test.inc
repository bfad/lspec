local(test_desc) = "[lspec_testSuite->getCurrentTestGroup] returns the current running test"
lspec_setupForTesting
local(where) = array
describe("1") => {
    #where->insert(lspec->getCurrentTestGroup->label)
    it("In 1") => { #where->insert(lspec->getCurrentTestGroup->label) }

    describe("1.1") => {
        #where->insert(lspec->getCurrentTestGroup->label)
        it("In 1.1") => { #where->insert(lspec->getCurrentTestGroup->label) }
        
        describe("1.1.1") => { #where->insert(lspec->getCurrentTestGroup->label) }
        describe("1.1.2") => { #where->insert(lspec->getCurrentTestGroup->label) }
    }
    
    describe("1.2") => {
        #where->insert(lspec->getCurrentTestGroup->label)
        it("In 1.2") => { #where->insert(lspec->getCurrentTestGroup->label) }
        
        describe("1.2.1") => { #where->insert(lspec->getCurrentTestGroup->label) }
        describe("1.2.2") => { #where->insert(lspec->getCurrentTestGroup->label) }
        describe("1.2.3") => { #where->insert(lspec->getCurrentTestGroup->label) }
    }
}
describe("2") => {
    #where->insert(lspec->getCurrentTestGroup->label)
    it("In 2") => { #where->insert(lspec->getCurrentTestGroup->label) }
    
    describe("2.1") => { #where->insert(lspec->getCurrentTestGroup->label) }
    describe("2.2") => { #where->insert(lspec->getCurrentTestGroup->label) }
    describe("2.3") => { #where->insert(lspec->getCurrentTestGroup->label) }
}
describe("3") => {
    #where->insert(lspec->getCurrentTestGroup->label)
    it("In 3") => { #where->insert(lspec->getCurrentTestGroup->label) }
}
local(expect) = array('1','1','1.1','1.1','1.1.1','1.1.2','1.2','1.2','1.2.1','1.2.2','1.2.3','2','2','2.1','2.2','2.3','3','3')
if(#expect != #where) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    stdoutnl('\nEXPECTED\n' + #expect)
    stdoutnl('\nRECEIVED\n' + #where)
    abort
}