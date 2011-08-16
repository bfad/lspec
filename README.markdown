LSpec
=====

LSpec is a [Test-Driven Development](http://en.wikipedia.org/wiki/Test-driven_development) tool for the Lasso language that gets its inspiration from [RSpec](https://github.com/rspec/rspec).


Terminology
-----------

<dl>
    <dt>Expectation</dt><dd>Used to define expected outcomes within a test. (Each [expect] statement)</dd>
    <dt>Test</dt><dd>A grouping of expectations and related code to test specific functionality. (Each [it] block.)</dd>
    <dt>Test Group</dt><dd>A grouping of related tests. (Each [describe] block.)</dd>
    <dt>Test Suite</dt><dd>Compilation of all the test groups.</dd>
</dl>


Installation
------------

### Pre-compiled Libraries

1. Click the big "Downloads" button next to the description on this page.
2. Choose the proper download for your platform
3. Decompress the file and move it into `$LASSO9_HOME/LassoLibraries/`

### Compile From Source

    $> cd where/you/want/the/source/installed/
    $> git clone https://github.com/bfad/lspec
    $> cd lspec
    $> make
    $> make install

_Note: If you're compiling on Mac OS X, you'll need the 10.5 SDK installed. You can follow the instructions [here](http://hints.macworld.com/article.php?story=20110318050811544) to restore the 10.5 SDK to Xcode 4._


Example
-------

First, create a file named zoo.test.inc and write a test describing functionality you wish you had. (It's important that the test file ends with the extension '.test.inc'.)

    describe('Zoo') => {
        describe('addAnimal') => {
            it('inserts an animal into the zoo and increases numberOfAnimals by 1') => {
                local(menagerie) = zoo
                local(num_animals_start) = #menagerie->numberOfAnimals
                local(num_animals_end)
                
                #menagerie->addAnimal(animal('Rhino'))
                #num_animals_end = #menagerie->numberOfAnimals
                
                expect(#menagerie->hasA('Rhino'))
                expect(#num_animals_end == 1 + #num_animals_start)
            }
        }
    }
    
    // Needed when running using lasso9
    // Will eventually run using an lspec execultable making this unnecessary.
    lspec->stop
    
Then run the test and watch it fail.

    $> lasso9 zoo.test.inc
    F

    Failures:

        1) Zoo addAnimal inserts an animal into the zoo and increases numberOfAnimals by 1
           Failure/Error: Unexpcted Error!
               Error Code: -9948
                Error Msg: Definition Not Found: zoo()
           # 3:9 /Path/To/zoo.test.inc

    Finished in 0.206136 seconds
    1 test, 1 failure

Now add the following code to the beginning of zoo.test.inc. (Eventually, there will be a command-line tool to help automate the running of tests so that tests can be in separate files.)
    
    define zoo => type {
        data private animals = array

        public hasA(name::string) => {
            with animal in .animals do {
                #name == #animal->name?
                    return true
            }
            return false
        }

        public addAnimal(creature::animal) => {
            .'animals'->insert(#creature)
        }

        public numberOfAnimals => .animals->size
    }

    define animal => type {
        data private name::string
        public name => .'name'

        public onCreate(name::string) => {
            .'name' = #name
        }
    }

With this code in place, re-run the test and watch it pass.

    $> lasso9 zoo.test.inc
    .

    Finished in 0.147116 seconds
    1 test, 0 failures


To Do
-----

1. Create more [expect] test case helpers.

2. Need before and after hooks for tests defined in the preceding test group(s).

3. Create command line tool to automatically run all tests in a directory.

4. Add color flag for terminal output.


License
-------

Copyright 2011 Bradley Lindsay

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

>    [http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0)

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.