require 'spec_helper'

describe Spyglass::Rect do
  let(:rect) { Spyglass::Rect.new(10, 20, 100, 200) }

  describe '.new' do
    it 'should require four arguments' do
      expect { Spyglass::Rect.new }.to raise_error ArgumentError
      expect( rect ).to be_a Spyglass::Rect
    end
  end

  describe 'accessors' do
    describe '#x' do
      it 'should return the correct X coordinate' do
        expect( rect.x ).to eq(10)
      end
    end

    describe '#y' do
      it 'should return the correct Y coordinate' do
        expect( rect.y ).to eq(20)
      end
    end

    describe '#width' do
      it 'should return the correct value' do
        expect( rect.width ).to eq(100)
      end
    end

    describe '#height' do
      it 'should return the correct value' do
        expect( rect.height ).to eq(200)
      end
    end

    describe '#area' do
      it 'should return `width * height`' do
        expect( rect.area ).to eq(20_000)
      end
    end

    describe '#size' do
      it 'should be an instance of Spyglass::Size' do
        expect( rect.size ).to be_a Spyglass::Size
      end

      it 'should return the correct values' do
        expect( rect.size.width ).to eq( rect.width )
        expect( rect.size.height ).to eq( rect.height )
      end
    end

    describe '#point' do
      it 'should be an instance of Spyglass::Point' do
        expect( rect.point ).to be_a Spyglass::Point
      end

      it 'should return the correct values' do
        expect( rect.point.x ).to eq( rect.x )
        expect( rect.point.y ).to eq( rect.y )
      end
    end

    describe '#center' do
      it 'should be an instance of Spyglass::Point' do
        expect( rect.center ).to be_a Spyglass::Point
      end

      it 'should return the correct values' do
        expect( rect.center.x ).to eq(60)
        expect( rect.center.y ).to eq(120)
      end
    end

    describe '#to_a' do
      it 'should be an instance of Array' do
        expect( rect.to_a ).to be_a Array
      end

      it 'should return the correct values' do
        expect( rect.to_a ).to eq([10, 20, 100, 200])
      end
    end

    describe '#contains?' do
      it 'should return true if the point exists within the rect' do
        pt = Spyglass::Point.new(50, 50)
        expect( rect.contains?(pt) ).to be_true
      end

      it 'should return false if the point only crosses the X axis' do
        pt = Spyglass::Point.new(20, 0)
        expect( rect.contains?(pt) ).to be_false
      end

      it 'should return false if the point only crosses the Y axis' do
        pt = Spyglass::Point.new(0, 50)
        expect( rect.contains?(pt) ).to be_false
      end
    end
  end

  describe 'setters' do
    describe '#height=' do
      it 'should set the value of the height' do
        rect.height = 150

        expect( rect.height ).to eq(150)
        expect( rect.area ).to eq(15_000)
      end
    end

    describe '#width=' do
      it 'should set the value of the width' do
        rect.width = 300

        expect( rect.width ).to eq(300)
        expect( rect.area ).to eq(60_000)
      end
    end

    describe '#x=' do
      it 'should set the value of the X coordinate' do
        rect.x = 20

        expect( rect.x ).to eq(20)
      end
    end

    describe '#y=' do
      it 'should set the value of the Y coordinate' do
        rect.y = 40

        expect( rect.y ).to eq(40)
      end
    end
  end

end
