import test from "node:test";
import assert from "node:assert/strict";
import { regionBounds } from "../src/mirrorRegions.js";

test("shared masks tile odd/even frames exactly without gaps or overlap", () => {
  for (const [width, height] of [[640,480], [81,61], [1920,1080]]) {
    for (const x of [10,33,50,79,90]) for (const y of [10,41,50,90]) {
      const plans = Array.from({length:4}, (_, i) => regionBounds(width,height,{x,y},i));
      assert.equal(plans.reduce((sum,p) => sum+p.w*p.h,0),width*height);
      assert.equal(plans[0].x+plans[0].w,plans[1].x);
      assert.equal(plans[0].y+plans[0].h,plans[2].y);
      for (const p of plans) {
        assert.ok(p.x >= 0 && p.y >= 0 && p.x+p.w <= width && p.y+p.h <= height);
        assert.ok(p.w>0 && p.h>0);
        assert.ok(Object.values(p).every(Number.isInteger));
      }
    }
  }
});
