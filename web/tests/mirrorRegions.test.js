import test from "node:test";
import assert from "node:assert/strict";
import { regionDrawPlan } from "../src/mirrorRegions.js";

test("shared boundaries cover odd/even frames exactly and source crops preserve proportions", () => {
  for (const [width, height] of [[640,480], [81,61], [1920,1080]]) {
    for (const x of [10,33,50,79,90]) for (const y of [10,41,50,90]) {
      for (const mirrored of [false,true]) for (const mirrorX of [false,true]) {
        const plans = Array.from({length:4}, (_, i) => regionDrawPlan(width,height,{x,y},i,mirrorX,mirrored));
        assert.equal(plans.reduce((sum,p) => sum+p.w*p.h,0),width*height);
        assert.equal(plans[0].x+plans[0].w,plans[1].x);
        assert.equal(plans[0].y+plans[0].h,plans[2].y);
        for (const p of plans) {
          assert.ok(p.sx >= -1e-8 && p.sy >= 0 && p.sx+p.sw <= width+1e-8 && p.sy+p.sh <= height+1e-8);
          assert.ok(Math.abs(p.sw/p.sh-p.w/p.h)<1e-8);
          assert.ok(p.sw>0 && p.sh>0);
        }
      }
    }
  }
});
