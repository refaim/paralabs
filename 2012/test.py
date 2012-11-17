import subprocess

count = 100

if __name__ == '__main__':
    for i in range(2, 5):
        result = set()
        task_number = str(i).zfill(2)
        for _ in range(count):
            process = subprocess.Popen('./task%s' % task_number, shell=True,
                stdout=subprocess.PIPE)
            process.wait()
            numbers, _ = process.communicate()
            result.add(numbers)
            if len(result) > 1:
                print 'Task %s is wrong' % task_number
                break
        print 'Task %s is correct' % task_number
