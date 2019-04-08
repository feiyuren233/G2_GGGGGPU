# python
# Run in tests dir

import difflib
import os
import subprocess

meow_pp_file_name = "../meow_pp.py"
test_dir = './files/'
prev_dir = test_dir + 'prev/'
temp_test_file_name = test_dir + 'temp.xx'

# Clear temp test file if existing
if os.path.exists(temp_test_file_name):
    os.remove(temp_test_file_name)

passed_count = 0
for root, dirs, files in os.walk(test_dir):
    if root == test_dir:
        for test_target_file in files:
            print test_target_file + ':'

            p = subprocess.Popen(['python3', meow_pp_file_name, test_dir + test_target_file,
                                  temp_test_file_name], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            run_out, run_err = p.communicate()
            if run_err:
                print(run_out)
                print(run_err)

            with open(temp_test_file_name) as temp_test_file:
                total_out = temp_test_file.read()

                for prev_root, prev_dirs, prev_files in os.walk(prev_dir):
                    if test_target_file not in prev_files:
                        print '    Error: ' + test_target_file + ' cannot be found in ' + prev_dir
                        print '           Maybe it is a new test, consider adding the expected output to prev!\n'
                        raise Exception(test_target_file +
                                        ' cannot be found in ' + prev_dir)

                with open(prev_dir + test_target_file, 'r') as prev_file:
                    prev_total_out = ''.join(prev_file.readlines())
                    if prev_total_out == total_out:
                        print '    Passed.'
                        passed_count += 1
                    else:
                        print '    Failed.'
                        print '    DIFF'
                        print '===== ACTUAL ====='
                        print total_out
                        print '=================='
                        print '***** EXPECTED *****'
                        print prev_total_out
                        print '********************'
                        print '----- DIFF -----'
                        total_out_lines = total_out.splitlines()
                        prev_total_out_lines = prev_total_out.splitlines()
                        line = 1
                        diff_count = 0
                        for total_out_line in total_out_lines:
                            if total_out_line != prev_total_out_lines[line-1]:
                                print "Line: " + str(line) + ":"
                                print "E:" + prev_total_out_lines[line-1]
                                print "A:" + total_out_line
                                diff_count += 1
                            line += 1
                            if diff_count == 10:
                                break
                        print '----------------'
                        print '\nThere are probably some bugs with the compiler, or update the EXPECTED in ' + prev_dir + test_target_file + '!'
                        print
                        raise Exception(
                            test_target_file + ' failed to produce same output as expected!')

            if os.path.exists(temp_test_file_name):
                os.remove(temp_test_file_name)

print '##########'
print 'Total Passed: ' + str(passed_count)
