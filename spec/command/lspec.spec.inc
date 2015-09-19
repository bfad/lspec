local(path_here) = currentCapture->callsite_file->stripLastComponent
not #path_here->beginsWith('/')
    ? #path_here = io_file_getcwd + '/' + #path_here
not #path_here->endsWith('/')
    ? #path_here->append('/')

local(lspec_path) = #path_here + '../../command/lspec.bin.inc'
var(lspec_path)   = #lspec_path

define call_lspec(args::staticarray=(:)) => {
    local(stdout, stderr)
    local(my_process) = sys_process
    
    handle => {#my_process->close}
    #my_process->open($lspec_path, #args, (:'PATH=$PATH:/usr/local/lasso'))
    #my_process->wait
    #stdout = #my_process->read
    #stderr = #my_process->readError
    
    return (:#stdout, #stderr)
}


var(termColor) = map(
    `normal`     = '' + decode_base64('Gw==') + '[0m',
    `boldRed`    = '' + decode_base64('Gw==') + '[1;31;49m',
    `boldGreen`  = '' + decode_base64('Gw==') + '[1;32;49m',
    `boldYellow` = '' + decode_base64('Gw==') + '[1;33;49m',
    `boldCyan`   = '' + decode_base64('Gw==') + '[1;36;49m',
)


describe('lspec command') => {
    
    describe('default lspec library options') => {
        local(the_options)
        beforeAll => {
            // Run it in a different thread and get the option values that get setup
            local(_,read) = split_thread => {
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
                
                var(argv) = (:#lspec_path->lastComponent, '//tmp/lspec_test_delete_me.spec.inc')
                file('//tmp/lspec_test_delete_me.spec.inc')->openTruncate&close
                protect => { sourcefile(file(#lspec_path))->invoke }
                #1->first->writeObject(lspec->options)
            }
            #the_options = #read->readObject
        }
        afterAll => {
            file('//tmp/lspec_test_delete_me.spec.inc')->delete
        }
        it(`is set to progressive mode`) => {
            expect('p', #the_options->find(`resultsFormat`))
        }
        it(`is set to output in color`) => {
            expect(true, #the_options->find(`colorOutput`))
        }
    }
    
    context('given a file path with no test groups') => {
        local(test_file) = file('/tmp/lspec.test')
        
        beforeEach => {
            #test_file->doWithClose => { #test_file->openTruncate }
        }
        
        afterEach => { #test_file->close&delete }
        
        it('executes the file') => {
            #test_file->doWithClose => {
                #test_file->writeString("file('" + #test_file->path + "')->openTruncate&close")
            }
            
            local(stdout, stderr) = call_lspec((:#test_file->path))
            
            expect(void == #stderr)
            expect('', #test_file->readString)
        }
        
        it('displays the "no test" summary') => {
            local(expected) = '\n\nFinished in 0.000000 seconds\n' + $termColor('boldCyan') + '0' + $termColor('normal') + ' tests, ' + $termColor('boldRed') + '0' + $termColor('normal') + ' failures\n\n'
            
            local(stdout, stderr) = call_lspec((:#test_file->path))
            
            expect(void == #stderr)
            expect(#expected, #stdout)
        }
    }
    
    context('given multiple file path with no test groups') => {
        local(test_file1) = file('/tmp/lspec.test1')
        local(test_file2) = file('/tmp/lspec.test2')
        local(test_file3) = file('/tmp/lspec.test3')
        
        beforeEach => {
            #test_file1->doWithClose => { #test_file1->openTruncate }
            #test_file2->doWithClose => { #test_file2->openTruncate }
            #test_file3->doWithClose => { #test_file3->openTruncate }
        }
        
        afterEach => {
            #test_file1->close&delete
            #test_file2->close&delete
            #test_file3->close&delete
        }

        it('executes each file') => {
            #test_file1->doWithClose => {
                #test_file1->writeString("file('" + #test_file1->path + "')->openTruncate&close")
            }
            #test_file2->doWithClose => {
                #test_file2->writeString("file('" + #test_file2->path + "')->openTruncate&close")
            }
            #test_file3->doWithClose => {
                #test_file3->writeString("file('" + #test_file3->path + "')->openTruncate&close")
            }
            
            local(stdout, stderr) = call_lspec((:#test_file1->path, #test_file2->path, #test_file3->path))
            
            expect(void == #stderr)
            expect('', #test_file1->readString)
            expect('', #test_file2->readString)
            expect('', #test_file3->readString)
        }

        it('displays the "no test" summary') => {
            local(expected) = '\n\nFinished in 0.000000 seconds\n' + $termColor('boldCyan') + '0' + $termColor('normal') + ' tests, ' + $termColor('boldRed') + '0' + $termColor('normal') + ' failures\n\n'

            local(stdout, stderr) = call_lspec((:#test_file1->path, #test_file2->path, #test_file3->path))

            expect(void == #stderr)
            expect(#expected, #stdout)
        }
    }
    
    context('given folder path(s)') => {
        beforeAll => {
            local(test_dir) = dir('/tmp/lspec_test/')
            #test_dir->create
            dir(#test_dir->path + 'spec/')->create
        }
        afterAll => {
            sys_process('/bin/rm', (:'-rf', #test_dir->path))->wait
        }
        afterEach => {
            sys_process('/bin/rm', (:'-rf', #test_dir->path + 'spec/'))->wait
            dir(#test_dir->path + 'spec/')->create
        }
        
        context('folder contains one file without the test extensions') => {
            beforeEach => {
                local(test_file) = file(#test_dir->path + 'spec/foo.lasso')
                #test_file->doWithClose => {
                    #test_file->writeString("file('" + #test_file->path + "')->openTruncate&close")
                }
            }
            
            it("doesn't execute the file") => {
                local(stdout, stderr) = call_lspec((:#test_dir->path + 'spec'))

                expect(void == #stderr)
                expect("file('" + #test_file->path + "')->openTruncate&close", #test_file->readString)
            }
            it('displays the "no test" summary') => {
                local(expected)       = '\n\nFinished in 0.000000 seconds\n' + $termColor('boldCyan') + '0' + $termColor('normal') + ' tests, ' + $termColor('boldRed') + '0' + $termColor('normal') + ' failures\n\n'
                local(stdout, stderr) = call_lspec((:#test_dir->path + 'spec'))
                
                expect(void == #stderr)
                expect(#expected, #stdout)
            }
        }
        
        context("folder containes a file with the test extension with no tests") => {
            beforeEach => {
                local(test_file) = file(#test_dir->path + 'spec/foo.spec.inc')
                #test_file->doWithClose => {
                    #test_file->writeString("file('" + #test_file->path + "')->openTruncate&close")
                }
            }
            
            it("executes the file") => {
                local(stdout, stderr) = call_lspec((:#test_dir->path + 'spec'))
                
                expect(void == #stderr)
                expect('', #test_file->readString)
            }
            it('displays the "no test" summary') => {
                local(expected)       = '\n\nFinished in 0.000000 seconds\n' + $termColor('boldCyan') + '0' + $termColor('normal') + ' tests, ' + $termColor('boldRed') + '0' + $termColor('normal') + ' failures\n\n'
                local(stdout, stderr) = call_lspec((:#test_dir->path + 'spec'))
                
                expect(void == #stderr)
                expect(#expected, #stdout)
            }
        }
        
        context("folder containes 3 files with the test extension with no tests") => {
            beforeEach => {
                local(test_file1) = file(#test_dir->path + 'spec/foo1.spec.inc')
                local(test_file2) = file(#test_dir->path + 'spec/foo2.spec.inc')
                local(test_file3) = file(#test_dir->path + 'spec/foo3.spec.inc')
                #test_file1->doWithClose => {
                    #test_file1->writeString("file('" + #test_file1->path + "')->openTruncate&close")
                }
                #test_file2->doWithClose => {
                    #test_file2->writeString("file('" + #test_file2->path + "')->openTruncate&close")
                }
                #test_file3->doWithClose => {
                    #test_file3->writeString("file('" + #test_file3->path + "')->openTruncate&close")
                }
            }
            
            it("executes the files") => {
                local(_, stderr) = call_lspec((:#test_dir->path + 'spec'))
                
                expect(void == #stderr)
                expect('', #test_file1->readString)
                expect('', #test_file2->readString)
                expect('', #test_file3->readString)
            }
            it('displays the "no test" summary') => {
                local(expected)       = '\n\nFinished in 0.000000 seconds\n' + $termColor('boldCyan') + '0' + $termColor('normal') + ' tests, ' + $termColor('boldRed') + '0' + $termColor('normal') + ' failures\n\n'
                local(stdout, stderr) = call_lspec((:#test_dir->path + 'spec'))
                
                expect(void == #stderr)
                expect(#expected, #stdout)
            }
        }
        
        context("folder contains a sub folder with an empty file that has the test extension") => {
            beforeEach => {
                dir(#test_dir->path + 'spec/sub/')->create
                local(test_file) = file(#test_dir->path + 'spec/sub/foo.spec.inc')
                #test_file->doWithClose => {
                    #test_file->writeString("file('" + #test_file->path + "')->openTruncate&close")
                }
            }
            
            it("executes the file") => {
                local(stdout, stderr) = call_lspec((:#test_dir->path + 'spec'))
                
                expect(void == #stderr)
                expect('', #test_file->readString)
            }
            it('displays the "no test" summary') => {
                local(expected)       = '\n\nFinished in 0.000000 seconds\n' + $termColor('boldCyan') + '0' + $termColor('normal') + ' tests, ' + $termColor('boldRed') + '0' + $termColor('normal') + ' failures\n\n'
                local(stdout, stderr) = call_lspec((:#test_dir->path + 'spec'))
                
                expect(void == #stderr)
                expect(#expected, #stdout)
            }
        }
        
        context("three folders each with a file with the test extension with no tests") => {
            beforeEach => {
                dir(#test_dir->path + 'spec/sub1/')->create
                dir(#test_dir->path + 'spec/sub2/')->create
                dir(#test_dir->path + 'spec/sub3/')->create

                local(test_file1) = file(#test_dir->path + 'spec/sub1/foo1.spec.inc')
                local(test_file2) = file(#test_dir->path + 'spec/sub2/foo2.spec.inc')
                local(test_file3) = file(#test_dir->path + 'spec/sub3/foo3.spec.inc')
                #test_file1->writeString("file('" + #test_file1->path + "')->openTruncate&close")&close
                #test_file2->writeString("file('" + #test_file2->path + "')->openTruncate&close")&close
                #test_file3->writeString("file('" + #test_file3->path + "')->openTruncate&close")&close
            }
            
            it("executes the files") => {
                local(_, stderr) = call_lspec((:#test_dir->path + 'spec/sub1', #test_dir->path + 'spec/sub2', #test_dir->path + 'spec/sub3'))
                
                expect(void == #stderr)
                expect('', #test_file1->readString)
                expect('', #test_file2->readString)
                expect('', #test_file3->readString)
            }
            it('displays the "no test" summary') => {
                local(expected)       = '\n\nFinished in 0.000000 seconds\n' + $termColor('boldCyan') + '0' + $termColor('normal') + ' tests, ' + $termColor('boldRed') + '0' + $termColor('normal') + ' failures\n\n'
                local(stdout, stderr) = call_lspec((:#test_dir->path + 'spec/sub1', #test_dir->path + 'spec/sub2', #test_dir->path + 'spec/sub3'))
                
                expect(void == #stderr)
                expect(#expected, #stdout)
            }
            
            context("sub-folders three levels deep in one of the folders that have files with the test extension") => {
                beforeEach => {
                    dir(#test_dir->path + 'spec/sub1/')->create
                    dir(#test_dir->path + 'spec/sub1/sub2/')->create
                    dir(#test_dir->path + 'spec/sub1/sub2/sub3/')->create

                    local(test_file1) = file(#test_dir->path + 'spec/sub1/sub2/sub3/foo1.spec.inc')
                    local(test_file2) = file(#test_dir->path + 'spec/sub1/sub2/sub3/foo2.spec.inc')
                    local(test_file3) = file(#test_dir->path + 'spec/sub1/sub2/sub3/foo3.spec.inc')
                    #test_file1->writeString("file('" + #test_file1->path + "')->openTruncate&close")&close
                    #test_file2->writeString("file('" + #test_file2->path + "')->openTruncate&close")&close
                    #test_file3->writeString("file('" + #test_file3->path + "')->openTruncate&close")&close
                }
                
                it("executes the files") => {
                    local(_, stderr) = call_lspec((:#test_dir->path + 'spec'))
                    
                    expect(void == #stderr)
                    expect('', #test_file1->readString)
                    expect('', #test_file2->readString)
                    expect('', #test_file3->readString)
                }
                it('displays the "no test" summary') => {
                    local(expected)       = '\n\nFinished in 0.000000 seconds\n' + $termColor('boldCyan') + '0' + $termColor('normal') + ' tests, ' + $termColor('boldRed') + '0' + $termColor('normal') + ' failures\n\n'
                    local(stdout, stderr) = call_lspec((:#test_dir->path + 'spec/sub1', #test_dir->path + 'spec/sub2', #test_dir->path + 'spec/sub3'))
                    
                    expect(void == #stderr)
                    expect(#expected, #stdout)
                }
            }
        }
    }
    
    context('called with no arguments') => {
        beforeAll => {
            local(test_dir) = dir('/tmp/lspec_test/')
            #test_dir->create
            dir(#test_dir->path + 'spec/')->create
        }
        afterAll => {
            sys_process('/bin/rm', (:'-rf', #test_dir->path))->wait
        }
        afterEach => {
            sys_process('/bin/rm', (:'-rf', #test_dir->path + 'spec/'))->wait
            dir(#test_dir->path + 'spec/')->create
        }
        
        it("throws an error if a folder named spec doesn't exist") => {
            // [dir->setCwd] can cause errors with lspec library, so
            // make sure to save off cwd first and set it back right after lspec call
            local(cwd)      = dir(io_file_getcwd)
            local(expected) = error_code_resNotFound + ':' + error_msg_resNotFound + ' - ./spec/' + sys_eol
            
            sys_process('/bin/rm', (:'-rf', #test_dir->path + 'spec/'))->wait
            
            #test_dir->setcwd
            handle => { #cwd->setcwd }
            local(stdout, stderr) = call_lspec
            #cwd->setcwd
            
            expect(#expected, #stderr)
            expect(void == #stdout)
        }
        it("executes all the files with the test extension in the spec folder") => {
            local(cwd)        = dir(io_file_getcwd)
            local(expected)   = '\n\nFinished in 0.000000 seconds\n' + $termColor('boldCyan') + '0' + $termColor('normal') + ' tests, ' + $termColor('boldRed') + '0' + $termColor('normal') + ' failures\n\n'
            local(test_file1) = file(#test_dir->path + 'spec/foo1.spec.inc')
            local(test_file2) = file(#test_dir->path + 'spec/foo2.spec.inc')
            local(test_file3) = file(#test_dir->path + 'spec/foo3.expt.inc')
            #test_file1->writeString("file('" + #test_file1->path + "')->openTruncate&close")&close
            #test_file2->writeString("file('" + #test_file2->path + "')->openTruncate&close")&close
            #test_file3->writeString("file('" + #test_file3->path + "')->openTruncate&close")&close
            
            #test_dir->setcwd
            handle => { #cwd->setcwd }
            local(stdout, stderr) = call_lspec
            #cwd->setcwd
            
            expect(void == #stderr)
            expect(#expected, #stdout)
            expect('', #test_file1->readString)
            expect('', #test_file2->readString)
            expect("file('" + #test_file3->path + "')->openTruncate&close", #test_file3->readString)
        }
        it("executes all the files with the test extension in any of spec's sub-folders") => {
            dir(#test_dir->path + 'spec/sub1/')->create
            dir(#test_dir->path + 'spec/sub1/sub2/')->create
            dir(#test_dir->path + 'spec/sub1/sub2/sub3/')->create

            local(test_file1) = file(#test_dir->path + 'spec/sub1/foo1.spec.inc')
            local(test_file2) = file(#test_dir->path + 'spec/sub1/sub2/foo2.spec.inc')
            local(test_file3) = file(#test_dir->path + 'spec/sub1/sub2/sub3/foo3.inc')
            #test_file1->writeString("file('" + #test_file1->path + "')->openTruncate&close")&close
            #test_file2->writeString("file('" + #test_file2->path + "')->openTruncate&close")&close
            #test_file3->writeString("file('" + #test_file3->path + "')->openTruncate&close")&close
            
            #test_dir->setcwd
            handle => { #cwd->setcwd }
            local(stdout, stderr) = call_lspec
            #cwd->setcwd
            
            expect(void == #stderr)
            expect(#expected, #stdout)
            expect('', #test_file1->readString)
            expect('', #test_file2->readString)
            expect("file('" + #test_file3->path + "')->openTruncate&close", #test_file3->readString)
        }
    }
    
    context('called with the -exts argument') => {
        it('displays the usage message when no extensions list is passed') => {
            local(stdout, stderr) = call_lspec((:'-exts'))
            
            expect(void == #stderr)
            expect(#stdout->beginsWith('usage:'))
        }
        it("executes the specified file even if it doesn't have the extension") => {
            local(f) = file('//tmp/lspec.test.rh')
            #f->doWithClose => {
                #f->writeString("file('" + #f->path + "')->openTruncate&close")
            }
            
            local(stdout, stderr) = call_lspec((:#f->path, '-exts', 'lasso,inc'))
            
            expect(void == #stderr)
            expect('', #f->readString)
        }
        it(`executes those files in a directory that end in .spec.[extension passed]`) => {
            local(test_dir) = dir('//tmp/lspec/')
            handle => { sys_process('/bin/rm', (:'-rf', #test_dir->path))->wait }
            #test_dir->create
            
            local(test_file1) = file(#test_dir->path + 'foo1.spec.inc')
            local(test_file2) = file(#test_dir->path + 'foo2.spec.lasso')
            local(test_file3) = file(#test_dir->path + 'foo3.lasso')
            #test_file1->writeString("file('" + #test_file1->path + "')->openTruncate&close")&close
            #test_file2->writeString("file('" + #test_file2->path + "')->openTruncate&close")&close
            #test_file3->writeString("file('" + #test_file3->path + "')->openTruncate&close")&close
            
            local(stdout, stderr) = call_lspec((:#test_dir->path, '-exts', 'lasso'))
            
            expect(void == #stderr)
            expect('', #test_file2->readString)
            expect("file('" + #test_file1->path + "')->openTruncate&close", #test_file1->readString)
            expect("file('" + #test_file3->path + "')->openTruncate&close", #test_file3->readString)
        }
        it(`executes those files in a directory that end in .spec.[any of the csv extensions passed]`) => {
            local(test_dir) = dir('//tmp/lspec/')
            handle => { sys_process('/bin/rm', (:'-rf', #test_dir->path))->wait }
            #test_dir->create
            
            local(test_file1) = file(#test_dir->path + 'foo1.spec.inc')
            local(test_file2) = file(#test_dir->path + 'foo2.spec.lasso')
            local(test_file3) = file(#test_dir->path + 'foo3.spec.rh')
            local(test_file4) = file(#test_dir->path + 'foo4.rh')
            #test_file1->writeString("file('" + #test_file1->path + "')->openTruncate&close")&close
            #test_file2->writeString("file('" + #test_file2->path + "')->openTruncate&close")&close
            #test_file3->writeString("file('" + #test_file3->path + "')->openTruncate&close")&close
            #test_file4->writeString("file('" + #test_file4->path + "')->openTruncate&close")&close
            
            local(stdout, stderr) = call_lspec((:#test_dir->path, '-exts', 'lasso,rh'))
            
            expect(void == #stderr)
            expect('', #test_file2->readString)
            expect('', #test_file3->readString)
            expect("file('" + #test_file1->path + "')->openTruncate&close", #test_file1->readString)
            expect("file('" + #test_file4->path + "')->openTruncate&close", #test_file4->readString)
        }
    }

    context('called with the -nocolor argument') => {
        local(file_testPass) = file('//tmp/lspec.test.pass')
        local(file_testFail) = file('//tmp/lspec.test.fail')
        local(file_testPend) = file('//tmp/lspec.test.pend')
        beforeAll => {
            #file_testPass->doWithClose => {
                #file_testPass->openTruncate&writeString("\
                    describe('passing test') => { it('passes') => { expect(true) } }\
                ")
            }
            #file_testFail->doWithClose => {
                #file_testFail->openTruncate&writeString("\
                    describe('failing test') => { it('fails') => { expect(false) } }\
                ")
            }
            #file_testPend->doWithClose => {
                #file_testPend->openTruncate&writeString("\
                    describe('pending test') => { it('pends') => { } }\
                ")
            }
        }
        afterAll => {
            #file_testPass->delete
            #file_testFail->delete
            #file_testPend->delete
        }
        it(`doesn't colorize passing tests`) => {
            local(expected) = regexp("\\.\n\n" + `Finished in [0-9.]+ seconds` + "\n1 test, 0 failures")
            local(stdout, stderr) = call_lspec((:'-nocolor', #file_testPass->path))
            
            expect(void == #stderr)
            expect(#stdout->asString->contains(#expected))
        }
        it(`doesn't colorize failing tests`) => {
            local(expected) = regexp(
                "F\n\nFailures:\n\n    1\\) failing test fails\n       Failure/Error: expect\\(false\\) \\} \\}\n" +
                "           expected: true\n                got: false\n       # 1:48 //tmp/lspec\\.test\\.fail\n\n" +
                `Finished in [0-9.]+ seconds` + "\n1 test, 1 failure"
            )
            local(stdout, stderr) = call_lspec((:'-nocolor', #file_testFail->path))

            expect(void == #stderr)
            expect(#stdout->asString->contains(#expected))
        }
        it(`doesn't colorize pending tests`) => {
            local(expected) = regexp(
                "\\*\n\nPending:\n\n    1\\) pending test pends\n           # 1:31 //tmp/lspec.test.pend\n\n" +
                `Finished in [0-9.]+ seconds` + "\n1 test, 0 failures, 1 pending"
            )
            local(stdout, stderr) = call_lspec((:'-nocolor', #file_testPend->path))

            expect(void == #stderr)
            expect(#stdout->asString->contains(#expected))
        }
    }

    context('called with the -documentation argument') => {
        local(file_testPass) = file('//tmp/lspec.test.pass')
        local(file_testFail) = file('//tmp/lspec.test.fail')
        local(file_testPend) = file('//tmp/lspec.test.pend')
        beforeAll => {
            #file_testPass->doWithClose => {
                #file_testPass->openTruncate&writeString("\
                    describe('passing test') => { it('passes') => { expect(true) } }\
                ")
            }
            #file_testFail->doWithClose => {
                #file_testFail->openTruncate&writeString("\
                    describe('failing test') => { it('fails') => { expect(false) } }\
                ")
            }
            #file_testPend->doWithClose => {
                #file_testPend->openTruncate&writeString("\
                    describe('pending test') => { it('pends') => { } }\
                ")
            }
        }
        afterAll => {
            #file_testPass->delete
            #file_testFail->delete
            #file_testPend->delete
        }
        it(`prints names of passing tests`) => {
            local(expected) = regexp("passing test\n    passes\n\n" + `Finished in [0-9.]+ seconds`)
            local(stdout, stderr) = call_lspec((:'-documentation', #file_testPass->path))

            expect(void == #stderr)
            expect(#stdout->asString->contains(#expected))
        }
        it(`prints names of failing tests`) => {
            local(expected) = regexp(
                "failing test\n    fails \\(FAILED - 1\\)\n\n" +
                string_replace($termColor('boldRed'), -find=`[`, -replace=`\[`) + "Failures:" +
                string_replace($termColor('normal'), -find=`[`, -replace=`\[`) + "\n\n" +
                "    1\\) failing test fails\n       Failure/Error: expect\\(false\\) \\} \\}\n" +
                "           expected: true\n                got: false\n       # 1:48 //tmp/lspec\\.test\\.fail\n\n" +
                `Finished in [0-9.]+ seconds`
            )
            local(stdout, stderr) = call_lspec((:'-documentation', #file_testFail->path))

            expect(void == #stderr)
            expect(#stdout->asString->contains(#expected))
        }
        it(`prints names of pending tests`) => {
            local(expected) = regexp(
                "pending test\n    pends\n\n" +
                string_replace($termColor('boldYellow'), -find=`[`, -replace=`\[`) + "Pending:" +
                string_replace($termColor('normal'), -find=`[`, -replace=`\[`) + "\n\n" +
                "    1\\) pending test pends\n           # 1:31 //tmp/lspec.test.pend\n\n" +
                `Finished in [0-9.]+ seconds`
            )
            local(stdout, stderr) = call_lspec((:'-documentation', #file_testPend->path))

            expect(void == #stderr)
            expect(#stdout->asString->contains(#expected))
        }
    }
}
