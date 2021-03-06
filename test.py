import unittest
from guillotineopt import layout2d


class TestCase(unittest.TestCase):
    def test_simple(self):
        self.maxDiff = None
        rect = {"size": (10, 10)}
        res = layout2d([rect], (10, 10))
        self.assertIn(res["along"], (0, 1))
        self.assertEqual(0, rect["amount"])
        if res["along"] == 0:
            self.assertDictEqual(
                {"along": 0, "elements": [{"type": 2, "rect": rect, "size": 10}]},
                res)
        else:
            self.assertDictEqual(
                {"along": 1, "elements": [{"type": 2, "rect": rect, "size": 10}]},
                res)

        # two identical rectangles along x axis
        rect = {"size": (5, 10), "amount": 2}
        self.assertDictEqual(
            {"along": 0, "elements": [{"type": 2, "rect": rect, "size": 5},
                                      {"type": 1, "size": 0},
                                      {"type": 2, "rect": rect, "size": 5}]},
            layout2d([rect], (10, 10)))

    def test_rectangle_with_remain(self):
        self.maxDiff = None
        rect = {"size": (5, 10), "amount": 2}
        self.assertDictEqual(
            {"along": 0, "elements": [{"type": 2, "rect": rect, "size": 5},
                                      {"type": 1, "size": 0},
                                      {"type": 2, "rect": rect, "size": 5},
                                      {"type": 1, "size": 0},
                                      {"type": 0, "size": 2},
                                      ]},
            layout2d([rect], (12, 10)))
        self.assertEqual(0, rect["amount"])

    def test_rectangle_with_remains(self):
        self.maxDiff = None
        rect = {"size": (5, 10), "amount": 2}
        det_layout = {"along": 0, "elements": [{"type": 2, "rect": rect, "size": 5},
                                               {"type": 1, "size": 0},
                                               {"type": 2, "rect": rect, "size": 5},
                                               ]}
        left_layout = {"along": 1, "elements": [
            {"type": 3, "size": 10, "layout": det_layout},
            {"type": 1, "size": 0},
            {"type": 0, "size": 1}]}
        self.assertDictEqual(
            {"along": 0, "elements": [
                {"type": 3, "size": 10, "layout": left_layout},
                {"type": 1, "size": 0},
                {"type": 0, "size": 2}
                ]
                },
            layout2d([rect], (12, 11)))
        self.assertEqual(0, rect["amount"])

    def test_skinny_detail(self):
        # +-+---------+
        # + +    2    +
        # + +         +
        # +1+---------+
        # + +    2    +
        # + +         +
        # + +---------+
        # +-+---------+
        rect1 = {"size": (1, 10)}
        rect2 = {"size": (9, 4), "amount": 2}
        res = layout2d([rect1, rect2], (10, 10))
        self.maxDiff = None
        self.assertDictEqual(
            {"along": 0, "elements": [
                {"type": 2, "rect": rect1, "size": 1},
                {"type": 1, "size": 0},
                {"type": 3, "layout": {
                    "along": 1, "elements": [
                        {"type": 2, "rect": rect2, "size": 4},
                        {"type": 1, "size": 0},
                        {"type": 2, "rect": rect2, "size": 4},
                        {"type": 1, "size": 0},
                        {"type": 0, "size": 2},
                        ]
                    }, "size": 9}]},
            res)
        self.assertEqual(0, rect1["amount"])
        self.assertEqual(0, rect2["amount"])

    def test_consumption_bug(self):
        rect1 = {"size": (1, 10)}
        rect2 = {"size": (9, 4)}
        res = layout2d([rect1, rect2], (10, 10))
        self.maxDiff = None
        self.assertDictEqual(
            {"along": 0, "elements": [
                {"type": 2, "rect": rect1, "size": 1},
                {"type": 1, "size": 0},
                {"type": 3, "layout": {
                    "along": 1, "elements": [
                    {"type": 2, "rect": rect2, "size": 4},
                    {"type": 1, "size": 0},
                    {"type": 0, "size": 6},
                    ]
                }, "size": 9}]},
            res)
        self.assertEqual(0, rect1["amount"])
        self.assertEqual(0, rect2["amount"])

    def test_rects_with_one_same_size(self):
        rect1 = {"size": (2, 4)}
        rect2 = {"size": (3, 4)}
        res = layout2d([rect1, rect2], (5, 4))
        self.assertEqual(0, rect1["amount"])
        self.assertEqual(0, rect2["amount"])

    def test_duplicate_details(self):
       rect1 = {"size": (4, 10), "name": "rect1"}
       rect2 = {"size": (4, 10), "name": "rect2"}
       self.assertDictEqual(
           {"along": 0, "elements": [{"type": 2, "rect": rect1, "size": 4},
                                     {"type": 1, "size": 0},
                                     {"type": 2, "rect": rect2, "size": 4}]},
           layout2d([rect1, rect2], (8, 10)))
       self.assertEqual(0, rect1["amount"])
       self.assertEqual(0, rect2["amount"])

    def test_large_parts(self):
        rect1 = {"size": (30, 30), "amount": 1}
        rect2 = {"size": (10, 5), "amount": 3}
        res = layout2d([rect1, rect2], (11, 5))
        self.assertDictEqual({"along": 0, "elements": [{"type": 2, "rect": rect2, "size": 10},
                                                       {"type": 1, "size": 0},
                                                       {"type": 0, "size": 1}]},
                             res)
        self.assertEqual(1, rect1["amount"])
        self.assertEqual(2, rect2["amount"])

    def test_zero_parts(self):
        rect1 = {"size": (10, 5), "amount": 0}
        rect2 = {"size": (5, 5), "amount": 3}
        res = layout2d([rect1, rect2], (10, 5))
        self.assertDictEqual({"along": 0, "elements": [{"type": 2, "rect": rect2, "size": 5},
                                                       {"type": 1, "size": 0},
                                                       {"type": 2, "rect": rect2, "size": 5}]},
                             res)
        self.assertEqual(0, rect1["amount"])
        self.assertEqual(1, rect2["amount"])

    def test_transposed_sublayout(self):
        rect1 = {"size": (3, 9)}
        rect2 = {"size": (7, 5), "amount": 2}
        rect3 = {"size": (2, 2)}
        res = layout2d([rect1, rect2, rect3], (10, 11))
        self.assertEqual(0, rect1["amount"])
        self.assertEqual(0, rect2["amount"])
        self.assertEqual(0, rect3["amount"])
        self.maxDiff = None
        left_bottom_layout = {
            "along": 0, "elements": [{"type": 2, "rect": rect3, "size": 2},
                                     {"type": 1, "size": 0},
                                     {"type": 0, "size": 1}]}
        left_layout = {"along": 1, "elements": [{"type": 2, "rect": rect1, "size": 9},
                                                {"type": 1, "size": 0},
                                                {"type": 3, "layout": left_bottom_layout, "size": 2}]}
        right_layout = {
            "along": 1, "elements": [{"type": 2, "rect": rect2, "size": 5},
                                     {"type": 1, "size": 0},
                                     {"type": 2, "rect": rect2, "size": 5},
                                     {"type": 1, "size": 0},
                                     {"type": 0, "size": 1}]}
        self.assertDictEqual(
            {"along": 0, "elements": [{"type": 3, "layout": left_layout, "size": 3},
                                      {"type": 1, "size": 0},
                                      {"type": 3, "layout": right_layout, "size": 7}]},
            res)

    def test_cut_size(self):
        rect = {"size": (2, 4), "amount": 2}
        res = layout2d([rect], (7, 4), cut_size=2)
        self.assertDictEqual(
            {"along": 0, "elements": [{"type": 2, "rect": rect, "size": 2},
                                      {"type": 1, "size": 2},
                                      {"type": 2, "rect": rect, "size": 2},
                                      {"type": 1, "size": 1}]},
            res)

    def test_detail_rotate(self):
        rect1 = {"size": (4, 2), "can_rotate": True}
        rect2 = {"size": (2, 4)}
        res = layout2d([rect1, rect2], (2, 8))
        self.assertDictEqual(
            {"along": 1, "elements": [{"type": 2, "rect": rect2, "size": 4},
                                      {"type": 1, "size": 0},
                                      {"type": 2, "rect": rect1, "size": 4},
                                      ]},
            res)
