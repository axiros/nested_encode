# nested_encode
Converts unicode objects, in a nested structure (eg. json.loads), into utf-8 encoded bytes.
For example:
```python

# Before
{u'a': [{'b': [(1, 2, [u'\xd6sterreich'])]}]}

# After
{'a': [{'b': [(1, 2, ['\xc3\x96sterreich'])]}]}
```

Note: dictionaries are copied, lists and tuples are changed *inplace*.

Performance overhead: around 10%.

# Usage
```python

import simplejson
from nested_encode import encode_nested


def loads(data):
    return encode_nested(simplejson.loads(data))
```

# Install
```bash
pip install git+https://github.com/axiros/nested_encode.git
```
