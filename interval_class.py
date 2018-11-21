class Interval:
    """
    This class implements an interval and can compare whether a given value inside the interval
    regular expression for valid INTEGER interval; Default: all values are valid
    e.g.: "[0, +10)", "(-10, 1998]", "[,]", ...
    """
    pattern = re.compile("^[\[(][ ]*([-+]?(([1-9][0-9]*)|0))?[ ]*,[ ]*([-+]?(([1-9][0-9]*)|0))?[ ]*[\])]$")

    def __init__(self, interval_str=None):
        # default settings: value should be a positive value
        self.left_value = None
        self.right_value = None

        self.left_is_exclusive = True
        self.right_is_exclusive = True

        if interval_str is None:
            return
        # strip the heading and trailing spaces first
        interval_str = interval_str.strip()
        if self.pattern.match(interval_str) is None:
            raise Exception("Input regular expression is invalid: " + str(interval_str) + "!")

        self.parse(interval_str)

    # parse the interval
    def parse(self, interval_str):
        left = interval_str[1:-1].split(',')[0].strip()
        right = interval_str[1:-1].split(',')[1].strip()

        # left and right are converted into long type!
        if left != '':
            if interval_str.startswith("["):
                self.left_is_exclusive = False
            self.left_value = long(left)
        if right != '':
            if interval_str.endswith("]"):
                self.right_is_exclusive = False
            self.right_value = long(right)

    # validate whether value is in the interval
    def validate(self, value):
        if (self.left_value is not None and value < self.left_value) or \
           (self.left_value is not None and self.left_is_exclusive and value == self.left_value) or \
           (self.right_value is not None and value > self.right_value) or \
           (self.right_value is not None and self.right_is_exclusive and value == self.right_value):
            return False

        return True

    # print error message
    def get_error_msg(self, field_name):
        left_comp = ""
        if self.left_value is not None:
            if self.left_is_exclusive:
                left_comp = " > " + str(self.left_value)
            else:
                left_comp = " >= " + str(self.left_value)
        right_comp = ""
        if self.right_value is not None:
            if self.right_is_exclusive:
                right_comp = " < " + str(self.right_value)
            else:
                right_comp = " <= " + str(self.right_value)
        link = ""
        if left_comp != "" and right_comp != "":
            link = " and"

        return "{} must be{}{}{}.".format(field_name, left_comp, link, right_comp)
