import test from "node:test";
import assert from "node:assert/strict";
import { StudySession, sessionFilename } from "../src/studySession.js";

function fixture() {
  let tick = 1000, time = Date.parse("2026-10-20T15:30:00.123Z");
  const session = new StudySession({ now: () => new Date(time), clock: () => tick, id: () => "example-session-id" });
  return { session, move: (milliseconds, wallMilliseconds = milliseconds) => { tick += milliseconds; time += wallMilliseconds; } };
}
const settings = () => ({ mirrored: true, center: { x: 32, y: 68 }, regions: { topLeft: { mirrorX: true } } });

test("before, during and after records retain participant/limb details and chart-friendly time/score columns", () => {
  const { session, move } = fixture();
  session.start({ participantName: "  Example Participant  ", limbLoss: [
    {side:"right",limb:"arm",level:"below elbow"}, {side:"left",limb:"leg",level:"above knee"},
  ], painScore: 6, viewSettings: settings() });
  move(60000); session.addScore(4, settings());
  move(120000); session.addScore(0, settings(), "after");
  const record = session.record;
  assert.equal(record.participantName, "Example Participant");
  assert.equal(record.limbLoss.length, 2);
  assert.equal(record.status, "completed");
  assert.equal(record.endedAt, "2026-10-20T15:33:00.123Z");
  assert.deepEqual(record.painEntries.map(({ phase, elapsedSeconds, painScore }) => [phase, elapsedSeconds, painScore]), [["before",0,6],["during",60,4],["after",180,0]]);
  assert.equal(record.painEntries.every(entry => entry.recordedAt.endsWith("Z")), true);
});

test("progress snapshots stay active and snapshots cannot mutate past measurements", () => {
  const {session,move} = fixture();
  const view = settings();
  session.start({painScore:0,viewSettings:view});
  view.center.x=75;
  move(2000);session.addScore(10,view);
  const progress=session.record;
  assert.equal(progress.status,"active");assert.equal(progress.endedAt,null);
  assert.equal(progress.painEntries[0].viewSettings.center.x,32);
  assert.equal(progress.painEntries[1].viewSettings.center.x,75);
  progress.participantName="mutated";progress.painEntries.length=0;
  assert.equal(session.record.participantName,"");assert.equal(session.record.painEntries.length,2);
});

test("invalid scores and illegal transitions cannot silently discard a session", () => {
  const {session}=fixture();
  assert.throws(()=>session.start({painScore:null,viewSettings:settings()}),/pain score/);
  assert.equal(session.record,null);
  session.start({painScore:5,viewSettings:settings()});
  for(const score of [-1,11,1.5,"5",NaN]) assert.throws(()=>session.addScore(score,settings()),/pain score/);
  assert.throws(()=>session.start({painScore:3,viewSettings:settings()}),/End the current/);
  assert.throws(()=>session.clear(),/End the current/);
  assert.throws(()=>session.addScore(3,settings(),"before"),/during or end/);
  assert.equal(session.record.painEntries.length,1);
  session.addScore(2,settings(),"after");
  assert.throws(()=>session.addScore(1,settings()),/Start a session/);
  session.clear();assert.equal(session.record,null);
});

test("elapsed time remains nondecreasing if wall clock changes", () => {
  const {session,move}=fixture();session.start({painScore:4,viewSettings:settings()});
  move(10000,-60000);session.addScore(5,settings());
  move(5000);session.addScore(3,settings(),"after");
  assert.deepEqual(session.record.painEntries.map(entry=>entry.elapsedSeconds),[0,10,15]);
});

test("multiple limbs include bilateral loss, and duplicate/invalid locations are rejected", () => {
  const {session}=fixture();
  const one={side:"left",limb:"leg",level:""};
  assert.throws(()=>session.start({painScore:3,limbLoss:[one,one],viewSettings:settings()}),/limb-loss/);
  assert.throws(()=>session.start({painScore:3,limbLoss:[{...one,side:"other"}],viewSettings:settings()}),/limb-loss/);
  session.start({painScore:3,limbLoss:[one,{...one,side:"right"}],viewSettings:settings()});
  assert.equal(session.record.limbLoss.length,2);
});

test("filenames start with a safe readable name or just the UTC date/time", () => {
  const {session}=fixture();session.start({participantName:"../Zoë / chair",painScore:1,viewSettings:settings()});
  assert.equal(sessionFilename(session.record),"Zoë_chair_2026-10-20T15-30-00-123Z.json");
  const record=session.record;record.participantName="";
  assert.equal(sessionFilename(record),"2026-10-20T15-30-00-123Z.json");
  record.participantName="椅".repeat(80);
  assert.ok(new TextEncoder().encode(sessionFilename(record)).length<255);
});
