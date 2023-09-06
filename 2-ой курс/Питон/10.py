class MealyError(Exception):
    pass


class Automate:
    def __init__(self):
        self.state = 'A'

    def check(self):
        if self.state == 'A':
            self.state = 'B'
            return 0
        raise MealyError('check')

    def visit(self):
        if self.state == 'A':
            self.state = 'D'
            return 1
        if self.state == 'B':
            self.state = 'C'
            return 3
        if self.state == 'E':
            self.state = 'H'
            return 7
        if self.state == 'F':
            self.state = 'A'
            return 9
        raise MealyError('visit')
        
    def view(self):
        if self.state == 'C':
            self.state = 'D'
            return 4
        if self.state == 'F':
            self.state = 'G'
            return 8
        if self.state == 'G':
            self.state = 'D'
            return 11
        raise MealyError('view')

    def play(self):
        if self.state == 'A':
            self.state = 'H'
            return 2
        if self.state == 'D':
            self.state = 'E'
            return 5
        if self.state == 'E':
            self.state = 'F'
            return 6
        if self.state == 'G':
            self.state = 'H'
            return 10
        raise MealyError('play')

def main():
    return Automate()

def test():
    states = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H']

    o = main()
    
    for state in states:
        try:
            o.state = state
            o.visit()

        except Exception as e:
            pass

    for state in states:
        try:
            o.state = state
            o.view()

        except Exception as e:
            pass

    for state in states:
        try:
            o.state = state
            o.play()

        except Exception as e:
            pass
        
    for state in states:
        try:
            o.state = state
            o.check()

        except Exception as e:
            pass