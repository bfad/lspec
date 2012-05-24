var(lspec_path) = io_file_getcwd + '/command/lspec'
define call_lspec(args::staticarray=(:)) => {
    local(stdout, stderr)
    local(my_process) = sys_process
    
    #my_process->open($lspec_path, #args)
    #my_process->wait
    #stdout = #my_process->read
    #stderr = #my_process->readError
    
    return (:#stdout, #stderr)
}

describe('lspec command') => {
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
            
            expect->null(#stderr)
            expect('', #test_file->readString)
        }
        
        it('displays the "no test" summary') => {
            local(expected) = '\n\nFinished in 0.000000 seconds\n0 tests, 0 failures\n'
            
            local(stdout, stderr) = call_lspec((:#test_file->path))
            
            expect->null(#stderr)
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
            
            expect->null(#stderr)
            expect('', #test_file1->readString)
            expect('', #test_file2->readString)
            expect('', #test_file3->readString)
        }

        it('displays the "no test" summary') => {
            local(expected) = '\n\nFinished in 0.000000 seconds\n0 tests, 0 failures\n'

            local(stdout, stderr) = call_lspec((:#test_file1->path, #test_file2->path, #test_file3->path))

            expect->null(#stderr)
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

                expect->null(#stderr)
                expect("file('" + #test_file->path + "')->openTruncate&close", #test_file->readString)
            }
            it('displays the "no test" summary') => {
                local(expected)       = '\n\nFinished in 0.000000 seconds\n0 tests, 0 failures\n'
                local(stdout, stderr) = call_lspec((:#test_dir->path + 'spec'))
                
                expect->null(#stderr)
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
                
                expect->null(#stderr)
                expect('', #test_file->readString)
            }
            it('displays the "no test" summary') => {
                local(expected)       = '\n\nFinished in 0.000000 seconds\n0 tests, 0 failures\n'
                local(stdout, stderr) = call_lspec((:#test_dir->path + 'spec'))
                
                expect->null(#stderr)
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
                
                expect->null(#stderr)
                expect('', #test_file1->readString)
                expect('', #test_file2->readString)
                expect('', #test_file3->readString)
            }
            it('displays the "no test" summary') => {
                local(expected)       = '\n\nFinished in 0.000000 seconds\n0 tests, 0 failures\n'
                local(stdout, stderr) = call_lspec((:#test_dir->path + 'spec'))
                
                expect->null(#stderr)
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
                
                expect->null(#stderr)
                expect('', #test_file->readString)
            }
            it('displays the "no test" summary') => {
                local(expected)       = '\n\nFinished in 0.000000 seconds\n0 tests, 0 failures\n'
                local(stdout, stderr) = call_lspec((:#test_dir->path + 'spec'))
                
                expect->null(#stderr)
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
                
                expect->null(#stderr)
                expect('', #test_file1->readString)
                expect('', #test_file2->readString)
                expect('', #test_file3->readString)
            }
            it('displays the "no test" summary') => {
                local(expected)       = '\n\nFinished in 0.000000 seconds\n0 tests, 0 failures\n'
                local(stdout, stderr) = call_lspec((:#test_dir->path + 'spec/sub1', #test_dir->path + 'spec/sub2', #test_dir->path + 'spec/sub3'))
                
                expect->null(#stderr)
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
                    
                    expect->null(#stderr)
                    expect('', #test_file1->readString)
                    expect('', #test_file2->readString)
                    expect('', #test_file3->readString)
                }
                it('displays the "no test" summary') => {
                    local(expected)       = '\n\nFinished in 0.000000 seconds\n0 tests, 0 failures\n'
                    local(stdout, stderr) = call_lspec((:#test_dir->path + 'spec/sub1', #test_dir->path + 'spec/sub2', #test_dir->path + 'spec/sub3'))
                    
                    expect->null(#stderr)
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
            local(expected) = error_code_resNotFound + ':' + error_msg_resNotFound
            
            sys_process('/bin/rm', (:'-rf', #test_dir->path + 'spec/'))->wait
            
            #test_dir->setcwd
            handle => { #cwd->setcwd }
            local(stdout, stderr) = call_lspec
            #cwd->setcwd
            
            expect(#expected, #stderr)
            expect->null(#stdout)
        }
        it("executes all the files with the test extension in the spec folder") => {
            local(cwd)        = dir(io_file_getcwd)
            local(expected)   = '\n\nFinished in 0.000000 seconds\n0 tests, 0 failures\n'
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
            
            expect->null(#stderr)
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
            
            expect->null(#stderr)
            expect(#expected, #stdout)
            expect('', #test_file1->readString)
            expect('', #test_file2->readString)
            expect("file('" + #test_file3->path + "')->openTruncate&close", #test_file3->readString)
        }
    }
}

lspec->stop