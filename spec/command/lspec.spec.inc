var(lspec_path) = 'command/lspec'
define call_lspec(args::staticarray=(:)) => {
    local(stdout, stderr)
    local(my_process) = sys_process
    
    #my_process->open($lspec_path, #args)
    #my_process->wait
    #stdout = #my_process->read
    #stderr = #my_process->readError
    
    return (:#stdout, #stderr)
}

describe('rspec command') => {
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
}

lspec->stop