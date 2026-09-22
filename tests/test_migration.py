import importlib.util
import json
from pathlib import Path
import tempfile
import unittest


SPEC = importlib.util.spec_from_file_location(
    "migrate_xml_to_json", Path(__file__).resolve().parents[1] / "scripts" / "migrate_xml_to_json.py"
)
migration = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(migration)


class MigrationTests(unittest.TestCase):
    def test_repeated_values_and_legacy_axis_name(self):
        with tempfile.TemporaryDirectory() as directory:
            source = Path(directory) / "synthetic.xml"
            source.write_text(
                "<FLYTHROUGH><RECORD id='0'><PARTICIPANT>synthetic</PARTICIPANT>"
                "<CAMERA>0</CAMERA><CAMERA>1</CAMERA>"
                "<POS_X>12.5</POS_X><POS_X>-4.25</POS_X>"
                "<ZAXIS_y>0.2</ZAXIS_y></RECORD></FLYTHROUGH>"
            )
            migration.migrate(source)
            destination = source.with_suffix(".json")
            record = json.loads(destination.read_text())["FLYTHROUGH"]["RECORD"]
            self.assertEqual(record["CAMERA"], [0, 1])
            self.assertEqual(record["POS_X"], [12.5, -4.25])
            self.assertEqual(record["ZAXIS_Y"], 0.2)
            self.assertEqual(record["PARTICIPANT"], "synthetic")
            self.assertTrue(source.exists())

            destination.write_text('{"keep": true}\n')
            migration.migrate(source)
            self.assertEqual(json.loads(destination.read_text()), {"keep": True})


if __name__ == "__main__":
    unittest.main()
